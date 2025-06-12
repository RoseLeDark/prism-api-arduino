#include "prism/prism.h"

prism_err _prism_arch_send_opcode(const prdev_t* device, const uint16_t op, const ui8 type, timeout_t timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }

    
    // Send the opcode to the device
    // Implementation depends on the specific architecture and communication protocol
    // This is a placeholder for the actual implementation
    return PR_OK; // Assume success for now
}
prism_err _prism_arch_send_opcode_arg1(const prdev_t* device, const uint16_t op, const ui8 type, const void* arg, timeout_t timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }

    
    // Send the opcode with an argument to the device
    // Implementation depends on the specific architecture and communication protocol
    // This is a placeholder for the actual implementation
    return PR_OK; // Assume success for now
}
prism_err _prism_send_bank_x(const _v256i vec, const bank_t bank, timeout_t timeout) {
  
    // Send the vector to the specified bank
    // Implementation depends on the specific architecture and communication protocol
    // This is a placeholder for the actual implementation
    return PR_OK; // Assume success for now
}

