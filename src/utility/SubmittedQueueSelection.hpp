#pragma once

#include <cstddef>
#include <cstdint>

namespace utility::d3d12 {
template<class Queue> struct QueueSelection {
    Queue* queue{};
    std::size_t matching_device{};
    std::size_t matching_thread{};
    bool retained{};
};

// A queue already proven for this swapchain remains valid when auxiliary
// queues begin submitting. Initial selection must still be unambiguous.
template<class Queue, class Entries, class MatchesDevice>
QueueSelection<Queue> select_submitted_queue(Queue* captured, bool same_swapchain,
    const Entries& entries, std::uint32_t current_thread, MatchesDevice matches_device) {
    if (captured != nullptr && same_swapchain && matches_device(captured)) {
        return {captured, 0, 0, true};
    }

    QueueSelection<Queue> result;
    Queue* sole{};
    Queue* same_thread{};
    for (const auto& entry : entries) {
        auto* queue = entry.queue.Get();
        if (!matches_device(queue)) {
            continue;
        }
        sole = queue;
        ++result.matching_device;
        if (entry.thread_id == current_thread) {
            same_thread = queue;
            ++result.matching_thread;
        }
    }
    result.queue = result.matching_thread == 1 ? same_thread :
        (result.matching_device == 1 ? sole : nullptr);
    return result;
}
}
