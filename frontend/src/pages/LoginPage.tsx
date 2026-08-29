/*
 * Displays the login form and authenticates predefined demo users.
 * Redirects the user to the correct dashboard after login.
 */

import {
  useState,
} from "react";

import {
  useNavigate,
} from "react-router";

import {
  login,
} from "../services/api";

import type {
  LoginUser,
} from "../services/api";


interface LoginPageProps {
  onLogin: (
    user: LoginUser
  ) => void | Promise<void>;
}


function LoginPage({
  onLogin,
}: LoginPageProps) {

  const navigate =
    useNavigate();


  const [
    username,
    setUsername,
  ] =
    useState("");


  const [
    password,
    setPassword,
  ] =
    useState("");


  const [
    loading,
    setLoading,
  ] =
    useState(false);


  const [
    error,
    setError,
  ] =
    useState("");


  const handleSubmit =
    async (
      event:
        React.FormEvent<HTMLFormElement>
    ) => {

      event.preventDefault();


      if (
        !username.trim() ||
        !password
      ) {

        setError(
          "Username and password are required."
        );

        return;
      }


      try {

        setLoading(
          true
        );

        setError(
          ""
        );


        const response =
          await login(
            username.trim(),
            password
          );


        // Store the logged-in user in App.
        await onLogin(
          response.user
        );


        // Redirect to the correct dashboard.
        navigate(
          response.user.role ===
          "STUDENT"
            ? "/student"
            : "/instructor",
          {
            replace: true,
          }
        );

      }
      catch (error) {

        setError(
          error instanceof Error
            ? error.message
            : "Login failed."
        );

      }
      finally {

        setLoading(
          false
        );
      }
    };


  return (
    <div className="login-page">

      <div className="login-container">

        <div className="login-header">

          <h1>
            University Automation
          </h1>

          <p>
            Course Registration
            and Grading System
          </p>

        </div>


        <form
          className="login-card"
          onSubmit={
            handleSubmit
          }
        >

          <div className="login-card-header">

            <h2>
              Sign In
            </h2>

            <p>
              Enter your predefined
              demo account credentials.
            </p>

          </div>


          <div className="login-form-group">

            <label
              htmlFor="username"
            >
              Username
            </label>

            <input
              id="username"
              type="text"
              value={
                username
              }
              placeholder="Enter username"
              autoComplete="username"
              disabled={
                loading
              }
              onChange={
                (event) =>
                  setUsername(
                    event.target.value
                  )
              }
            />

          </div>


          <div className="login-form-group">

            <label
              htmlFor="password"
            >
              Password
            </label>

            <input
              id="password"
              type="password"
              value={
                password
              }
              placeholder="Enter password"
              autoComplete="current-password"
              disabled={
                loading
              }
              onChange={
                (event) =>
                  setPassword(
                    event.target.value
                  )
              }
            />

          </div>


          {error && (

            <div className="login-error">
              {error}
            </div>

          )}


          <button
            className="login-submit-button"
            type="submit"
            disabled={
              loading
            }
          >

            {loading
              ? "Signing in..."
              : "Sign In"}

          </button>


          <div className="demo-credentials">

            <p>
              <strong>
                Demo Accounts
              </strong>
            </p>

            <span>
              undergrad / 1234
            </span>

            <span>
              graduate / 1234
            </span>

            <span>
              instructor / 1234
            </span>

          </div>

        </form>

      </div>

    </div>
  );
}


export default LoginPage;