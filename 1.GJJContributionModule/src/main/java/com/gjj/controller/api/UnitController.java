package com.gjj.controller.api;

import com.gjj.common.validator.IdCardValidator;
import com.gjj.dto.req.module.UnitAddReq;
import com.gjj.dto.req.module.UnitFilterReq;
import com.gjj.dto.req.module.UnitUpdateReq;
import com.gjj.dto.res.module.UnitFilterRes;
import com.gjj.entity.Session;
import com.gjj.entity.Unit;
import com.gjj.entity.User;
import com.gjj.service.Result;
import com.gjj.service.SessionService;
import com.gjj.service.SessionServiceImpl;
import com.gjj.service.impl.SysSequenceServiceImpl;
import com.gjj.service.impl.UnitServiceImpl;
import com.gjj.service.impl.UserServiceImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDateTime;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/api/units")
public class UnitController {
    private final SysSequenceServiceImpl seqSvc;
    private final UnitServiceImpl unitSvc;
    private final UserServiceImpl userSvc;

    @Autowired
    public UnitController(
            final SysSequenceServiceImpl seqSvc,
            final UnitServiceImpl unitSvc,
            final UserServiceImpl userSvc
    ) {
        this.seqSvc = seqSvc;
        this.unitSvc = unitSvc;
        this.userSvc = userSvc;
    }

    /**
     * 获取单位公积金账号信息
     * @param sessionId 请求头 会话 ID
     * @param unitAccNumber 单位公积金账号
     * @return JSON 格式单位公积金账号
     */
    @GetMapping("/{unitAccNumber}")
    public ResponseEntity<?> getBaseInfo(
            @RequestHeader("sessionId") final String sessionId,
            @PathVariable("unitAccNumber") final String unitAccNumber
    ) {
        // 1. 检查 session 有效性
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }

        // 2. 验证请求方的角色权限
        Session session = sessionSvc.getSession(sessionId);
        User requester = userSvc.getUserByUsername(session.getUsername()).data();
        if (!requester.getUserType().equals("admin")) {
            return ResponseEntity
                    .status(HttpStatus.FORBIDDEN)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. You lack ADMIN privileges."
                    ));
        }

        // 3. 获取数据
        Unit unit = new Unit();
        try {
            unit = unitSvc.getUnitByAccNumber(unitAccNumber);
        } catch (Exception e) {
            e.printStackTrace();
        }

        if(unit == null) {
            return ResponseEntity.status(HttpStatus.NOT_FOUND)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 查询目标 " + unitAccNumber + " 不存在"
                    ));
        }

        return ResponseEntity.status(HttpStatus.OK)
                .body(Map.of(
                        "result", true,
                        "message", "成功",
                        "data", unit
                ));
    }

    /**
     * 根据单位名称等信息筛选单位公积金账号
     * @param sessionId 会话 ID
     * @param req 筛选条件请求体
     * @return 结果 + 信息 + 数据（若存在）
     */
    @GetMapping
    public ResponseEntity<?> ListUnitsByName(
            @RequestHeader("sessionId") final String sessionId,
            @RequestBody final UnitFilterReq req
    ){
        // 1. 检查会话有效性
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }

        // 2. 列出单位信息
        List<Unit> units = unitSvc.searchUnitsByName(req.getUnitName());
        if(units == null || units.isEmpty()) {
            return ResponseEntity.status(HttpStatus.NOT_FOUND)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 没有查询到任何符合的数据."
                    ));
        }
        UnitFilterRes resp = new UnitFilterRes();
        resp.setTotalItems(units.size());
        resp.setUnits(units);
        return ResponseEntity.ok(Map.of(
                "result", true,
                "message", "成功",
                "data", resp
        ));
    }

    /**
     * 新增用户账号
     * @param sessionId 会话 ID
     * @param req 请求数据
     * @return 包括 unitAccNumber 在内的数据信息
     */
    @PostMapping
    public ResponseEntity<?> addUnit(
            @RequestHeader("sessionId") final String sessionId,
            @RequestBody final UnitAddReq req
    ){
        // 1. 检查会话有效性
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }
        final Session session = sessionSvc.getSession(sessionId);
        if(session == null || session.getUsername() == null ||session.getUsername().isEmpty()){
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话错误，无法查询到会话数据."
                    ));
        }

        // 2. 检查数据有效性
        if(!IdCardValidator.isValid(req.getAgentId())){
            return ResponseEntity.status(HttpStatus.BAD_REQUEST)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，经办人身份证号码不合法."
                    ));
        }

        // 3. 补全缺失数据
        final String unitAccNumber = seqSvc.getNextSequenceValue("unit");
        final String operator = session.getUsername();  // username, not userId
        final LocalDateTime now = LocalDateTime.now();

        // 4. 新增单位数据
        Unit unit = new Unit(
                unitAccNumber,
                req.getUnitName(),
                req.getUnitAddress(),
                req.getOrgCode(),
                req.getUnitCategory(),
                req.getUnitType(),
                req.getSalaryDate(),
                req.getUnitPhone(),
                req.getContactPerson(),
                req.getAgentId(),
                req.getAccountStatus(),
                req.getFundBalance(),
                req.getDepositBase(),
                req.getUnitRatio(),
                req.getEmployeeRatio(),
                req.getUnitMonthlyDue(),
                req.getEmployeeMonthlyDue(),
                req.getEmployeeCount(),
                req.getLastPaymentMonth(),
                req.getInstitutionCode(),
                operator,
                now,
                req.getRemark()
        );

        // 5. 返回结果
        Result.CommonResult ret = unitSvc.createUnit(unit);
        return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                .body(Map.of(
                        "result", ret.result(),
                        "message", ret.message()
                ));
    }

    /**
     * 根据 unitAccNumber 删除单位公积金账号信息
     * @param sessionId 会话信息
     * @param unitAccNumber 单位账号
     * @return JSON 格式 结果 + 信息 + 其他数据
     */
    @DeleteMapping("/{unitAccNumber}")
    public ResponseEntity<?> deleteUnit(
            @RequestHeader("sessionId") final String sessionId,
            @PathVariable("unitAccNumber") final String unitAccNumber
    ){
        // 1. 检查会话有效性
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }

        // 2. 执行删除操作
        Result.CommonResult ret = unitSvc.deleteUnit(unitAccNumber);
        return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                .body(Map.of(
                        "result", ret.result(),
                        "message", ret.message()
                ));
    }

    /**
     * 更新账号信息
     * @param sessionId 会话 ID
     * @param unitAccNumber 单位账号
     * @param req 请求数据
     * @return JSON 格式 结果 + 状态 + 其他信息
     */
    @PutMapping("/{unitAccNumber}")
    public ResponseEntity<?> updateUnit(
            @RequestHeader("sessionId") final String sessionId,
            @PathVariable("unitAccNumber") final String unitAccNumber,
            @RequestBody UnitUpdateReq req
    ){
        // 1. 检查会话有效性
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }

        // 2.执行更新操作
        Unit unit = new Unit(
                unitAccNumber,
                req.getUnitName(),
                req.getUnitAddress(),
                req.getOrgCode(),
                req.getUnitCategory(),
                req.getUnitType(),
                req.getSalaryDate(),
                req.getUnitPhone(),
                req.getContactPerson(),
                req.getAgentId(),
                req.getAccountStatus(),
                req.getFundBalance(),
                req.getDepositBase(),
                req.getUnitRatio(),
                req.getEmployeeRatio(),
                req.getUnitMonthlyDue(),
                req.getEmployeeMonthlyDue(),
                req.getEmployeeCount(),
                req.getLastPaymentMonth(),
                req.getInstitutionCode(),
                null,
                LocalDateTime.now(),
                req.getRemark()
        );
        Result.CommonResult ret = unitSvc.updateUnit(unit);
        return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                .body(Map.of(
                        "result", ret.result(),
                        "message", ret.message()
                ));
    }


}
