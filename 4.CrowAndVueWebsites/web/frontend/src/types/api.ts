// types/api.ts
export interface RegisterRequest {
  username: string;
  nickname: string;
  password: string;
  email: string;
  gender: string;
}

export interface RegisterResponse {
  message: string;
  username?: string;
  user_id?: number;
}

export interface LoginRequest {
  username?: string;
  email?: string;
  password: string;
}

export interface LoginResponse {
  gender: string;
  email: string;
  nickname: string;
  username: string;
  auth_method: string;
  user_id: number;
  session_id: string;
  message: string;
}
