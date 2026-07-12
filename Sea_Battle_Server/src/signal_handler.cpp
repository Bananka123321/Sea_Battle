#include "../include/signal_handler.h"

std::atomic<bool> SignalHandler::shutdown_requested_{false};

void SignalHandler::HandleSignal(int s) {
    shutdown_requested_.store(true, std::memory_order_release);
}

void SignalHandler::Setup() {
    struct sigaction sigHandler;

    sigHandler.sa_handler = HandleSignal;
    sigemptyset(&sigHandler.sa_mask);
    sigHandler.sa_flags = 0;

    sigaction(SIGINT, &sigHandler, nullptr);
    sigaction(SIGTERM, &sigHandler, nullptr);
}

bool SignalHandler::IsShutdownRequested() {
    return shutdown_requested_.load(std::memory_order_acquire);
}