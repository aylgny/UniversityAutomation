/*
 * Root application component.
 * Defines application routes and protects them by user role.
 */

import {
  useState,
} from "react";

import {
  Navigate,
  Route,
  Routes,
} from "react-router";

import "./App.css";

import LoginPage from
  "./pages/LoginPage";

import StudentDashboard from
  "./pages/student/StudentDashboard";

import InstructorDashboard from
  "./pages/instructor/InstructorDashboard";

import type {
  LoginUser,
} from "./services/api";


function App() {

  // Currently logged-in user.
  const [
    currentUser,
    setCurrentUser,
  ] =
    useState<LoginUser | null>(
      null
    );


  // =======================================================
  // LOGIN
  // =======================================================

  const handleLoginCompleted =
    (
      user: LoginUser
    ) => {

      setCurrentUser(
        user
      );
    };


  // =======================================================
  // LOGOUT
  // =======================================================

  const handleLogout =
    () => {

      setCurrentUser(
        null
      );
    };


  // =======================================================
  // ROUTES
  // =======================================================

  return (

    <Routes>

      {/* Login */}
      <Route
        path="/login"
        element={
          currentUser === null
            ? (
                <LoginPage
                  onLogin={
                    handleLoginCompleted
                  }
                />
              )
            : (
                <Navigate
                  to={
                    currentUser.role ===
                    "STUDENT"
                      ? "/student"
                      : "/instructor"
                  }
                  replace
                />
              )
        }
      />


      {/* Student routes */}
      <Route
        path="/student/*"
        element={
          currentUser !== null &&
          currentUser.role ===
          "STUDENT"
            ? (
                <StudentDashboard
                  user={
                    currentUser
                  }
                  onLogout={
                    handleLogout
                  }
                />
              )
            : (
                <Navigate
                  to="/login"
                  replace
                />
              )
        }
      />


      {/* Instructor routes */}
      <Route
        path="/instructor/*"
        element={
          currentUser !== null &&
          currentUser.role ===
          "INSTRUCTOR"
            ? (
                <InstructorDashboard
                  user={
                    currentUser
                  }
                  onLogout={
                    handleLogout
                  }
                />
              )
            : (
                <Navigate
                  to="/login"
                  replace
                />
              )
        }
      />


      {/* Unknown routes */}
      <Route
        path="*"
        element={
          <Navigate
            to={
              currentUser === null
                ? "/login"
                : currentUser.role ===
                  "STUDENT"
                  ? "/student"
                  : "/instructor"
            }
            replace
          />
        }
      />

    </Routes>
  );
}


export default App;