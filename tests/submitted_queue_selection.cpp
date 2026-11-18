#include <cassert>
#include <iostream>
#include <vector>
#include "utility/SubmittedQueueSelection.hpp"
struct Queue { int device; bool valid=true; };
struct Ref { Queue* q; Queue* Get() const { return q; } };
struct Entry { Ref queue; unsigned thread_id; };
int main() {
    Queue game{1}, aux_a{1}, aux_b{1}, other{2}, invalid{1, false};
    auto match = [](Queue* q) { return q->valid && q->device==1; };
    std::vector<Entry> entries{{{&game},10}};
    auto r=utility::d3d12::select_submitted_queue((Queue*)nullptr,false,entries,10,match);
    assert(r.queue==&game && !r.retained);
    entries.push_back({{&aux_a},10}); entries.push_back({{&aux_b},10});
    // RE8 regression: two VR helper queues appear after initial game Present.
    r=utility::d3d12::select_submitted_queue(&game,true,entries,10,match);
    assert(r.queue==&game && r.retained);
    // Initial ambiguous queues and a fresh/reset swapchain cannot reuse a pin.
    r=utility::d3d12::select_submitted_queue((Queue*)nullptr,false,entries,10,match);
    assert(r.queue==nullptr && r.matching_device==3 && r.matching_thread==3);
    r=utility::d3d12::select_submitted_queue(&game,false,entries,10,match);
    assert(r.queue==nullptr);
    // A device change invalidates the old pin; only the new device may win.
    entries={{{&other},10}};
    auto match2=[](Queue* q) { return q->valid && q->device==2; };
    r=utility::d3d12::select_submitted_queue(&game,true,entries,10,match2);
    assert(r.queue==&other && !r.retained);
    // Failed GetDevice is not a valid cache hit.
    entries.clear();
    r=utility::d3d12::select_submitted_queue(&invalid,true,entries,10,match);
    assert(r.queue==nullptr);
    // Preserve existing unique-thread selection among device-compatible queues.
    entries={{{&game},10},{{&aux_a},11},{{&other},10}};
    r=utility::d3d12::select_submitted_queue((Queue*)nullptr,false,entries,10,match);
    assert(r.queue==&game && r.matching_device==2 && r.matching_thread==1);
    // Even a different submitting thread does not invalidate an established pin.
    r=utility::d3d12::select_submitted_queue(&game,true,entries,11,match);
    assert(r.queue==&game && r.retained);
    std::cout << "PASS: initial selection, RE8 auxiliary queues, reset/swapchain change, device change, failed identity, thread matching\n";
}
