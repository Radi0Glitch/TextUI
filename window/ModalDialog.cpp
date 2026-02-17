#include "../window/ModalWindowManager.h"
#include "../window/Dialog.h"
#include "../core/Renderer.h"

// ModalDialog implementation
ModalDialog::ModalDialog(int x, int y, int width, int height, const std::string& title)
    : Dialog(x, y, width, height, title), completed(false), resultReady(false) {}

void ModalDialog::waitForResult() {
    std::unique_lock<std::mutex> lock(resultMutex);
    resultCondition.wait(lock, [this]() { return completed.load(); });
}

void ModalDialog::completeWithResult() {
    {
        std::lock_guard<std::mutex> lock(resultMutex);
        completed.store(true);
        resultReady = true;
    }
    resultCondition.notify_one();
}
