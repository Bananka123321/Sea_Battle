#pragma once

#include <csignal>
#include <atomic>

class SignalHandler {
public:
    static void Setup();
    static bool IsShutdownRequested();

private:
    static std::atomic<bool> shutdown_requested_;

private:
    static void HandleSignal(int signum);
};