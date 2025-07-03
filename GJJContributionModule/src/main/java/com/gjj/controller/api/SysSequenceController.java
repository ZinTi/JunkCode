package com.gjj.controller.api;

import com.gjj.dto.req.module.SysSequenceAddReq;
import com.gjj.dto.req.module.SysSequenceUpdateReq;
import com.gjj.entity.Session;
import com.gjj.entity.SysSequence;
import com.gjj.entity.User;
import com.gjj.service.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.Map;

@RestController
@RequestMapping("/api/sequence")
public class SysSequenceController {
    private final SysSequenceService sysSeqSvc;
    private final UserService userSvc;

    @Autowired
    public SysSequenceController(
            SysSequenceService sysSeqSvc,
            UserService userSvc
    ) {
        this.sysSeqSvc = sysSeqSvc;
        this.userSvc = userSvc;
    }

    @GetMapping("/{seqName}")
    public ResponseEntity<?> getSysSequenceUnit(
            @RequestHeader("sessionId") String sessionId,
            @PathVariable("seqName") String seqName
    ){
        // 1. 验证 session 有效性
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }

        // 2. 验证权限

        // 3. 获取数据
        SysSequence sysSequence = sysSeqSvc.getSysSequence(seqName);
        return ResponseEntity.ok(Map.of(
                "result", true,
                "message", "成功",
                "data",sysSequence
        ));
    }

    /**
     * 新增 序列记录 （只有管理员可以新增数据）
     * @param sessionId 会话 ID
     * @param req 数据
     * @return 新增结果
     */
    @PostMapping
    public ResponseEntity<?> addSysSequence(
            @RequestHeader("sessionId") String sessionId,
            @RequestBody SysSequenceAddReq req
    ){
        // 1. 验证 session 有效性
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }

        // 2. 检查权限
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

        // 3. 新增序列
        Result.CommonResult ret = sysSeqSvc.addSysSequence(
            new SysSequence(
                req.getSeqName(),
                req.getCurrentVal(),
                req.getMaxVal(),
                req.getDescription(),
                req.getRemark()
            )
        );
        if(!ret.result()){
            return ResponseEntity
                    .status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body(Map.of(
                            "result", false,
                            "message", "新增失败：" + ret.message()
                    ));
        }

        return ResponseEntity.ok(Map.of(
                "result", true,
                "message", "新增成功"
        ));
    }

    /**
     * 请求更新系统序列值表的数据
     * @param sessionId 会话 ID
     * @param seqName 更新目标
     * @param req 新数据
     * @return 结果
     */
    @PutMapping("/{seqName}")
    public ResponseEntity<?> updateSysSequence(
            @RequestHeader("sessionId") String sessionId,
            @PathVariable("seqName") String seqName,
            @RequestBody SysSequenceUpdateReq req
    ){
        // 1. 验证 session 有效性
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }

        // 2. 验证身份
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

        // 3. 更新操作
        SysSequence sysSequence = new SysSequence(
            seqName,
            req.getCurrentVal(),
            req.getMaxVal(),
            req.getDescription(),
            req.getRemark()
        );
        Result.CommonResult updateResult = sysSeqSvc.updateSysSequence(sysSequence);
        if(!updateResult.result()){
            return ResponseEntity.status(HttpStatus.INTERNAL_SERVER_ERROR)
                    .body(Map.of(
                            "result", false,
                            "message", updateResult.message()
                    ));
        }
        return ResponseEntity.ok(Map.of(
                "result", true,
                "message", updateResult.message()
        ));
    }

    /**
     * 删除系统序列值（危险操作）
     * @param sessionId 会话ID
     * @param seqName 选项
     * @return 结果
     */
    @DeleteMapping("/{seqName}")
    public ResponseEntity<?> deleteSysSequence(
            @RequestHeader("sessionId") String sessionId,
            @PathVariable String seqName
    ){
        // 1. 验证 session 有效性
        SessionService sessionSvc = new SessionServiceImpl();
        if (!sessionSvc.isValidSession(sessionId)) {
            return ResponseEntity
                    .status(HttpStatus.UNAUTHORIZED)
                    .body(Map.of(
                            "result", false,
                            "message", "FAILED. 会话过期或无效，请重新登录."
                    ));
        }

        // 2. 验证身份
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

        // 3. 删除操作
        Result.CommonResult result = sysSeqSvc.removeSysSequence(seqName);
        return ResponseEntity.ok(result);
    }

}
