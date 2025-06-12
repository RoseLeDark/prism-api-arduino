#ifndef __PRISM__      
#define __PRISM__   1

// prism.h - Header file for the Prism library
#define PRISM_VERSION_MAJOR 1
#define PRISM_VERSION_MINOR 0
#define PRISM_VERSION_PATCH 0
#define PRISM_VERSION_STRING "1.0.0"
#define PRISM_VERSION ((PRISM_VERSION_MAJOR << 16) | (PRISM_VERSION_MINOR << 8) | PRISM_VERSION_PATCH)
#define PRISM_VERSION_HEX ((PRISM_VERSION_MAJOR << 24) | (PRISM_VERSION_MINOR << 16) | (PRISM_VERSION_PATCH << 8))
// Prism library version macros
#define PRISM_VERSION_CHECK(major, minor, patch) \
    ((PRISM_VERSION_MAJOR == (major)) && (PRISM_VERSION_MINOR == (minor)) && (PRISM_VERSION_PATCH == (patch)))

//opcodes for PRISM SIMD
#define PRISM_OPCODE_TYPE_UI32 (0xD0)
#define PRISM_OPCODE_TYPE_SI32 (0xD1)
#define PRISM_OPCODE_TYPE_UI16 (0xD2)
#define PRISM_OPCODE_TYPE_SI16 (0xD3)
#define PRISM_OPCODE_TYPE_UI8  (0xD4)
#define PRISM_OPCODE_TYPE_SI8  (0xD5)
#define PRISM_OPCODE_TYPE_UI4  (0xDF)
#define PRISM_OPCODE_TYPE_SI4  (0xDE)
#if PRISM_ENABLE_64BIT_EXT == 1
#define PRISM_OPCODE_TYPE_UI64  (0xC0)
#if PRISM_ENABLE_LONGTYPES == 1
#define PRISM_OPCODE_TYPE_UI128 (0xC2)
#define PRISM_OPCODE_TYPE_UI256 (0xC4)
#endif // PRISM_ENABLE_LONGTYPES
#endif // PRISM_ENABLE_64BIT_EXT

#ifdef PRISM_ENABLE_IEE754
#define PRISM_OPCODE_TYPE_FL   (0xF0)
#define PRISM_OPCODE_TYPE_HF   (0xF1)
#endif
#define PRISM_OPCODE_TYPE_END  (0xFF) // End of type opcode sequence

#define PRISM_OPCODE_ADD_BASE (0x01) // vec1 = 00 bis vec8 = 07 
#define PRISM_OPCODE_SUB_BASE (0x08) //vec1 = 08 bis vec8 = 0F
#define PRISM_OPCODE_MUL_BASE (0x10) //vec1 = 10 bis vec8 = 17
#define PRISM_OPCODE_DIV_BASE (0x18) //vec1 = 18 bis vec8 = 1F
#define PRISM_OPCODE_AND_BASE (0x20) //vec1 = 20 bis vec8 = 27
#define PRISM_OPCODE_NAND_BASE  (0x28) //vec1 = 28 bis vec8 = 2F
#define PRISM_OPCODE_OR_BASE (0x30) //vec1 = 30 bis vec8 = 37
#define PRISM_OPCODE_XOR_BASE (0x38) //vec1 = 38 bis vec8 = 3F
#define PRISM_OPCODE_NOR_BASE (0x40) //vec1 = 40 bis vec8 = 47
#define PRISM_OPCODE_NOT_BASE (0x48) //vec1 = 48 bis vec8 = 4F

#define PRISM_OPCODE_NOTC (0x50) 
#define PRISM_OPCODE_CTOB (0x51) // From bank C to bank B
#define PRISM_OPCODE_CTOA (0x52) // From bank C to bank A

#define PRISM_OPCODE_STORE_A (0x58) //vector to bank a
#define PRISM_OPCODE_STORE_B (0x59) //vector to bank b
#define PRISM_OPCODE_LOAD_C (0x5A) //bank c to vector
#define PRISM_OPCODE_LOAD_D (0x5B) //bank d to vector
#define PRISM_OPCODE_LOAD_A (0x5C) //bank a to vector
#define PRISM_OPCODE_LOAD_B (0x5D) //bank b to vector

#define PRISM_OPCODE_END (0x5F) // End of opcode sequence -- internal use only, not for public use
#define PRISM_OPCODE_NOCLEAR_AFTEROP (0x60) // Do not clear bank A and B after operation
#define PRISM_OPCODE_CLEAR_AFTEROP (0x61) // Clear bank A and B after operation
#define PRISM_OPCODE_CLEAR_C (0x62) // Clear bank C 
#define PRISM_OPCODE_CLEAR_D (0x63) // Clear bank D 
#define PRISM_OPCODE_CLEAR_ALL (0x64) // Clear all banks A, B, C, D 

#define PRISM_OPCODE_CMP_EQ (0x70) // Compare equal
#define PRISM_OPCODE_CMP_NE (0x71) // Compare not equal
#define PRISM_OPCODE_CMP_GT (0x72) // Compare greater than
#define PRISM_OPCODE_CMP_GE (0x73) // Compare greater than or equal
#define PRISM_OPCODE_CMP_LT (0x74) // Compare less than
#define PRISM_OPCODE_CMP_LE (0x75) // Compare less than or equal
#define PRISM_OPCODE_CPL2 (0x76)   // 2's complement

#define PRISM_OPCODE_SHIFT_L (0x77)   // Shift left
#define PRISM_OPCODE_SHIFT_R (0x78)   // Shift right

#if __cplusplus
extern "C" {
#endif // __cplusplus
#include <stdint.h>

#if PRISM_ENABLE_64BIT_EXT == 1
typedef uint64_t ui64;
typedef int64_t si64;
#if PRISM_ENABLE_LONGTYPES == 1
typedef uint128_t ui128;
typedef int128_t si128;
#endif // PRISM_ENABLE_LONGTYPES
#endif // PRISM_ENABLE_64BIT_EXT

typedef enum {
    PR_OK= 0,
    PR_ERR_INVALID_ARGUMENT,
    PR_ERR_OUT_OF_MEMORY,
    PR_ERR_UNSUPPORTED_OPERATION,
    PR_ERR_UNKNOWN
} prism_err;

/**
 * @brief Represents a 256-bit vector with 8 entries of 32-bit unsigned integers.
 * Each entry can be accessed as a union of different types.
 * This structure is used for SIMD operations in the Prism architecture.
 * The vector can hold 8 entries of 32-bit unsigned integers, 16 entries of 16-bit unsigned integers,
 * or 32 entries of 8-bit unsigned integers, both signed and unsigned.
 * The structure is designed to be compatible with SIMD operations and allows for efficient data manipulation.
 * The entries can be accessed using the `ui`, `si`, `uix`, `six`, `uib`, and `sib` fields.
 * The `ui` field is an array of 8 unsigned 32-bit integers, while `si` is an array of 8 signed 32-bit integers.
 * The `uix` field is an array of 16 unsigned 16-bit integers, and `six` is an array of 16 signed 16-bit integers.
 * The `uib` field is an array of 32 unsigned 8-bit integers, and `sib` is an array of 32 signed 8-bit integers.
 * This structure is used in the Prism architecture for SIMD operations and can be extended for additional types.
 * @note The structure is designed to be compatible with SIMD operations and allows for efficient data manipulation.
 */
typedef struct prism256i_type {
    union {
        ui32 ui[8];
        int32_t si[8];
        uint16_t uix[16];
        int16_t six[16];
        uint8_t uib[32]; // 4x8-bit unsigned integers
        int8_t sib[32]; // 4x8-bit signed integers        
    };
} _v256i;

typedef struct prism_dev_type prdev_t;

/**
 * @brief Enum representing the banks of the Prism device.
 * Each bank corresponds to a specific memory area in the Prism architecture.
 * The banks are used to store and retrieve data during SIMD operations.
 * The enum values are used to identify the banks when performing operations such as loading or storing vectors.
 */
typedef enum prism_bank_enum {
    PRISM_BANK_A = 0x00, // Bank A
    PRISM_BANK_B = 0x01, // Bank B
    PRISM_BANK_C = 0x02, // Bank C
    PRISM_BANK_D = 0x03,  // Bank D

    PRISM_BANK_MAX = 0x04 // Maximum bank value, used for validation
} bank_t;


typedef uint8_t   ui8;
typedef uint16_t ui16;
typedef uint32_t ui32;

typedef int8_t   si8;
typedef int16_t si16;
typedef int32_t si32;

typedef ui32  timeout_t;

/**
 * @brief Sends an opcode to the Prism device.
 * This function is used to send a specific operation code (opcode) to the Prism device.
 * The opcode determines the operation to be performed on the device, such as loading or storing data.
 * The function takes a pointer to the device, the opcode to be sent, and a timeout value.
 * The timeout value specifies the maximum time to wait for the operation to complete.
 * If the operation is successful, the function returns PR_OK. If there is an error, it returns an appropriate error code.
 * @param device Pointer to the Prism device structure.
 * @param op The operation code to be sent to the device. 
 * @param timeout The timeout value in milliseconds. If the operation does not complete within this time, it will return an error.
 * @note The function is designed to be used with the Prism architecture and is not intended for general-purpose use.
 * @return 
 */
extern prism_err _prism_arch_send_opcode(const prdev_t* device, const uint16_t op, const ui8 type, timeout_t timeout);

extern prism_err _prism_arch_send_opcode_arg1(const prdev_t* device, const uint16_t op, const ui8 type, const void* arg, timeout_t timeout);
/**
 * @brief Sends a 256-bit vector to the specified bank of the Prism device.
 * This function is used to send a 256-bit vector to either bank A or bank B of the Prism device.
 * The vector is represented as a 256-bit integer, which can hold 8 entries of 32-bit unsigned integers.
 * The function takes a pointer to the vector, the bank to which the vector should be sent, and a timeout value.
 * The timeout value specifies the maximum time to wait for the operation to complete.
 * If the operation is successful, the function returns PR_OK. If there is an error, it returns an appropriate error code.
 * The function is designed to be used with the Prism architecture and is not intended for general-purpose use.
 * @param vec A 256-bit vector represented as a union of different types. 
 * @param bank The bank to which the vector should be sent. It can be either PRISM_BANK_A or PRISM_BANK_B.
 *            The bank is used to determine where the vector will be stored in the Prism device.
 *            The bank_t enum defines the available banks in the Prism architecture.
 * @param timeout The timeout value in milliseconds. If the operation does not complete within this time, it will return an error. 
 * @return prism_err
 *         Returns PR_OK if the operation is successful, or an error code if there is an issue.
 * @note This function is an internal function and should not be used directly by users of the Prism library.  
 */
extern prism_err _prism_send_bank_x(const _v256i vec, const bank_t bank, timeout_t timeout);

/**
 * @brief Loads a 256-bit vector from the specified bank of the Prism device.
 * This function is used to load a 256-bit vector from either bank A, B, C, or D of the Prism device.
 * The vector is represented as a pointer to a 256-bit integer, which can hold 8 entries of 32-bit unsigned integers.
 * The function takes the bank from which the vector should be loaded and a timeout value.
 * The timeout value specifies the maximum time to wait for the operation to complete.
 * If the operation is successful, it returns a pointer to the loaded vector. If there is an error, it returns NULL.
 * The function is designed to be used with the Prism architecture and is not intended for general-purpose use.
 * @param bank The bank from which the vector should be loaded. It can be either PRISM_BANK_A, PRISM_BANK_B, PRISM_BANK_C, or PRISM_BANK_D.
 * @param timeout The timeout value in milliseconds. If the operation does not complete within this time, it will return an error.
 * @return A pointer to the loaded 256-bit vector if successful, or NULL if there is an error.
 */
extern _v256i* _prism_load_bank_x(const bank_t bank, timeout_t timeout);

/**
 * @brief Sets a 256-bit vector with 8 entries of 32-bit unsigned integers.
 * This function initializes a 256-bit vector with the specified 8 entries of unsigned integers.
 * The vector can be used for SIMD operations in the Prism architecture.
 * The function takes 8 unsigned integers as input and returns a 256-bit vector.
 * @param a, b, c, d, e, f, g, h The 8 unsigned integers to be set in the vector.
 * @return A 256-bit vector initialized with the specified values.
 */
static inline _v256i _v256_set8_uiv(const ui32 a, const ui32 b, const ui32 c, const ui32 d,
                                     const ui32 e, const ui32 f, const ui32 g, const ui32 h) {
    _v256i v;
    v.ui[0] = a; v.ui[1] = b;  v.ui[2] = c; v.ui[3] = d;
    v.ui[4] = e; v.ui[5] = f; v.ui[6] = g; v.ui[7] = h;
    return v;
}

#define _v256_set7_uiv(a, b, c, d, e, f, g)    _v256_set8_uiv(a, b, c, d, e, f, g, 0) 
#define _v256_set6_uiv(a, b, c, d, e, f)       _v256_set8_uiv(a, b, c, d, e, f, 0, 0) 
#define _v256_set5_uiv(a, b, c, d, e)          _v256_set8_uiv(a, b, c, d, e, 0, 0, 0) 
#define _v256_set4_uiv(a, b, c, d)             _v256_set8_uiv(a, b, c, d, 0, 0, 0, 0)
#define _v256_set3_uiv(a, b, c)                _v256_set8_uiv(a, b, c, 0, 0, 0, 0, 0)
#define _v256_set2_uiv(a, b)                   _v256_set8_uiv(a, b, 0, 0, 0, 0, 0, 0)
#define _v256_set1_uiv(a)                      _v256_set8_uiv(a, 0, 0, 0, 0, 0, 0, 0)

#if PRISM_ENABLE_64BIT_EXT == 1
inline static inline _v256i _v256_set_uiv64(const ui64 a, const ui64 b, const ui64 c, const ui64 d) {
    union {
        ui64 x;
        ui32 hl[2];
    } _a _b _c _d;
    _a.x = a; _b.x = b; _c.x = c; _d.x = d;
    return _v256_set8_uiv(_a.hl[0], _a.hl[1], _b.hl[0], _b.hl[1],
                   _c.hl[0], _c.hl[1], _d.hl[0], _d.hl[1]);
}   
#endif // PRISM_ENABLE_64BIT_EXT

#if PRISM_ENABLE_LONGTYPES == 1

inline static inline _v256i _v256_set_uiv128(const ui128 a, const ui128 b) {
    union {
        ui128 x;
        ui32 hl[4];
    } _a _b;
    _a.x = a; _b.x = b;
    return _v256_set8_uiv(_a.hl[0], _a.hl[1], _a.hl[2], _a.hl[3],
                          _b.hl[0], _b.hl[1], _b.hl[2], _b.hl[3]);
}   

inline static inline _v256i _v256_set_uiv128(const ui256 a) {
    union {
        ui256 x;
        ui32 hl[8];
    } _a;
    _a.x = a;;
    return _v256_set8_uiv(_a.hl[0], _a.hl[1], _a.hl[2], _a.hl[3],
                          _a.hl[4], _a.hl[5], _a.hl[6], _a.hl[7]);
}  
#endif // PRISM_ENABLE_LONGTYPES


/**
 * @brief Sets a 256-bit vector with 8 entries of 32-bit signed integers.
 * This function initializes a 256-bit vector with the specified 8 entries of signed integers.
 * The vector can be used for SIMD operations in the Prism architecture.
 * The function takes 8 signed integers as input and returns a 256-bit vector.
 * @param a, b, c, d, e, f, g, h The 8 signed integers to be set in the vector.
 * @return A 256-bit vector initialized with the specified values.
 */
static inline _v256i _v256_set8_siv(const si32 a, const si32 b, const si32 c, const si32 d,
                                     const si32 e, const si32 f, const si32 g, const si32 h) {
    _v256i v;
    v.si[0] = a; v.si[1] = b;  v.si[2] = c; v.si[3] = d;
    v.si[4] = e; v.si[5] = f; v.si[6] = g;  v.si[7] = h;
    return v;
}

#define _v256_set7_uiv(a, b, c, d, e, f, g)    _v256_set8_siv(a, b, c, d, e, f, g, 0) 
#define _v256_set6_uiv(a, b, c, d, e, f)       _v256_set8_siv(a, b, c, d, e, f, 0, 0) 
#define _v256_set5_uiv(a, b, c, d, e)          _v256_set8_siv(a, b, c, d, e, 0, 0, 0) 
#define _v256_set4_uiv(a, b, c, d)             _v256_set8_siv(a, b, c, d, 0, 0, 0, 0)
#define _v256_set3_uiv(a, b, c)                _v256_set8_siv(a, b, c, 0, 0, 0, 0, 0)
#define _v256_set2_uiv(a, b)                   _v256_set8_siv(a, b, 0, 0, 0, 0, 0, 0)
#define _v256_set1_uiv(a)                      _v256_set8_siv(a, 0, 0, 0, 0, 0, 0, 0)

/**
 * @brief Stores a 256-bit vector to the specified bank (A or B) of the Prism device.
 * This function sends an opcode to the device to store the vector in the specified bank.
 * The vector is represented as a 256-bit integer, which can hold 8 entries of 32-bit unsigned integers.
 */
static inline prism_err _v256_store_bank_v(const prdev_t* device, const bank_t bank, const _v256i vec, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT;  }
    if(bank != PRISM_BANK_A && bank != PRISM_BANK_B) { return PR_ERR_INVALID_ARGUMENT;  }

    prism_err err = _prism_arch_send_opcode(device, 
        bank == PRISM_BANK_A ? PRISM_OPCODE_STORE_A : PRISM_OPCODE_STORE_B, 
        PRISM_OPCODE_TYPE_UI32, timeout);

    if (err != PR_OK) { return err; }
    return  _prism_send_bank_x( vec, bank,  timeout);
}

#define _v256_store_bank_a(device, vec, timeout) _v256_store_bank_v(device, PRISM_BANK_A, vec, timeout)
#define _v256_store_bank_b(device, vec, timeout) _v256_store_bank_v(device, PRISM_BANK_B, vec, timeout)


/**
 * @brief Loads a 256-bit vector from the specified bank (A, B, C, or D) of the Prism device.
 * @param device Pointer to the Prism device structure. 
 * @param bank The bank from which the vector should be loaded. It can be either PRISM_BANK_A, PRISM_BANK_B, PRISM_BANK_C, or PRISM_BANK_D. 
 * @param timeout The timeout value in milliseconds. If the operation does not complete within this time, it will return an error. 
 * @param err Pointer to a prism_err variable to store the error code if any error occurs. 
 * @return A pointer to the loaded 256-bit vector if successful, or NULL if there is an error. 
 */
static inline _v256i* _v256_load_bank_x(const prdev_t* device, const bank_t bank,  ui32 timeout, prism_err* err) {
    if (device == 0) { if(err) *err = PR_ERR_INVALID_ARGUMENT; }
    if(bank > bank_t::PRISM_BANK_MAX) { if(err) *err = PR_ERR_INVALID_ARGUMENT;  }

    prism_err _ret = PR_OK;

    if(bank == PRISM_BANK_A) {
        _ret = _prism_arch_send_opcode(device, PRISM_OPCODE_LOAD_A, PRISM_OPCODE_TYPE_UI32,timeout); 
    } else if(bank == PRISM_BANK_B) {
        _ret = _prism_arch_send_opcode(device, PRISM_OPCODE_LOAD_B, PRISM_OPCODE_TYPE_UI32,timeout);
    } else if(bank == PRISM_BANK_C) {
        _ret = _prism_arch_send_opcode(device, PRISM_OPCODE_LOAD_C, PRISM_OPCODE_TYPE_UI32,timeout);
    } else if(bank == PRISM_BANK_D) {
        _ret = _prism_arch_send_opcode(device, PRISM_OPCODE_LOAD_D, PRISM_OPCODE_TYPE_UI32,timeout);
    } else {
        _ret =  PR_ERR_INVALID_ARGUMENT;
    }
    if(_ret == PR_OK) {
        _v256i* vec = _prism_load_bank_x(bank, timeout);
        if(err) *err = PR_OK;
        return vec;
    } 
    if(err) *err = _ret;
    return 0;
}

#define _v256_load_bank_a(device, timeout, err) _v256_load_bank_x(device, PRISM_BANK_A, timeout, err)
#define _v256_load_bank_b(device, timeout, err) _v256_load_bank_x(device, PRISM_BANK_B, timeout, err)
#define _v256_load_bank_c(device, timeout, err) _v256_load_bank_x(device, PRISM_BANK_C, timeout, err)
#define _v256_load_bank_d(device, timeout, err) _v256_load_bank_x(device, PRISM_BANK_D, timeout, err)

static inline prism_err _v256_store_ctob(const prdev_t* device, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CTOB, PRISM_OPCODE_TYPE_UI32,timeout);
}

static inline prism_err _v256_store_ctoa(const prdev_t* device, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CTOA, PRISM_OPCODE_TYPE_UI32, timeout);
}

static inline prism_err _v256_set_caop(const prdev_t* device, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CLEAR_AFTEROP, PRISM_OPCODE_TYPE_UI32, timeout);
}

static inline prism_err _v256_set_ncaop(const prdev_t* device, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    return _prism_arch_send_opcode(device, PRISM_OPCODE_NOCLEAR_AFTEROP, PRISM_OPCODE_TYPE_UI32, timeout);
}

static inline prism_err _v256_run_clear_c(const prdev_t* device, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CLEAR_C, PRISM_OPCODE_TYPE_UI32, timeout);
}
static inline prism_err _v256_run_clear_d(const prdev_t* device, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CLEAR_D, PRISM_OPCODE_TYPE_UI32, timeout);
}
// Clear all banks A, B, C, D
static inline prism_err _v256_run_clear_all(const prdev_t* device, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CLEAR_ALL, PRISM_OPCODE_TYPE_UI32, timeout);
}


static inline prism_err _v256_addN_ui32(const prdev_t* device, const ui8 type, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }

    return _prism_arch_send_opcode(device, PRISM_OPCODE_ADD_BASE + (vector_len % 8), type, timeout);
}

#define _v256_add8_ui32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_UI32, 8, timeout)
#define _v256_add7_ui32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_UI32, 7, timeout)
#define _v256_add6_ui32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_UI32, 6, timeout)
#define _v256_add5_ui32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_UI32, 5, timeout)
#define _v256_add4_ui32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_UI32, 4, timeout)
#define _v256_add3_ui32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_UI32, 3, timeout)
#define _v256_add2_ui32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_UI32, 2, timeout)
#define _v256_add1_ui32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_UI32, 1, timeout)

#define _v256_add8_si32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_SI32, 8, timeout)
#define _v256_add7_si32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_SI32, 7, timeout)
#define _v256_add6_si32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_SI32, 6, timeout)
#define _v256_add5_si32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_SI32, 5, timeout)
#define _v256_add4_si32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_SI32, 4, timeout)
#define _v256_add3_si32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_SI32, 3, timeout)
#define _v256_add2_si32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_SI32, 2, timeout)
#define _v256_add1_si32(device, timeout) _v256_addN_ui32(device, PRISM_OPCODE_TYPE_SI32, 1, timeout)

static inline prism_err _v256_subN_ui32(const prdev_t* device, const ui8 type, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }

    return _prism_arch_send_opcode(device, PRISM_OPCODE_SUB_BASE + (vector_len % 8), type, timeout);
}
#define _v256_sub8_ui32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_UI32, 8, timeout)
#define _v256_sub7_ui32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_UI32, 7, timeout)
#define _v256_sub6_ui32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_UI32, 6, timeout)
#define _v256_sub5_ui32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_UI32, 5, timeout)
#define _v256_sub4_ui32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_UI32, 4, timeout)
#define _v256_sub3_ui32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_UI32, 3, timeout)
#define _v256_sub2_ui32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_UI32, 2, timeout)
#define _v256_sub1_ui32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_UI32, 1, timeout)
#define _v256_sub8_si32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_SI32, 8, timeout)
#define _v256_sub7_si32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_SI32, 7, timeout)
#define _v256_sub6_si32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_SI32, 6, timeout)
#define _v256_sub5_si32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_SI32, 5, timeout)
#define _v256_sub4_si32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_SI32, 4, timeout)
#define _v256_sub3_si32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_SI32, 3, timeout)
#define _v256_sub2_si32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_SI32, 2, timeout)
#define _v256_sub1_si32(device, timeout) _v256_subN_ui32(device, PRISM_OPCODE_TYPE_SI32, 1, timeout)

static inline prism_err _v256_mulN_ui32(const prdev_t* device, const ui8 type, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }

   return _prism_arch_send_opcode(device, PRISM_OPCODE_MUL_BASE + (vector_len % 8), type, timeout);
}
#define _v256_mul8_ui32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_UI32, 8, timeout)
#define _v256_mul7_ui32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_UI32, 7, timeout)
#define _v256_mul6_ui32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_UI32, 6, timeout)
#define _v256_mul5_ui32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_UI32, 5, timeout)
#define _v256_mul4_ui32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_UI32, 4, timeout)
#define _v256_mul3_ui32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_UI32, 3, timeout)
#define _v256_mul2_ui32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_UI32, 2, timeout)
#define _v256_mul1_ui32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_UI32, 1, timeout)
#define _v256_mul8_si32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_SI32, 8, timeout)
#define _v256_mul7_si32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_SI32, 7, timeout)
#define _v256_mul6_si32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_SI32, 6, timeout)
#define _v256_mul5_si32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_SI32, 5, timeout)
#define _v256_mul4_si32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_SI32, 4, timeout)
#define _v256_mul3_si32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_SI32, 3, timeout)
#define _v256_mul2_si32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_SI32, 2, timeout)
#define _v256_mul1_si32(device, timeout) _v256_mulN_ui32(device, PRISM_OPCODE_TYPE_SI32, 1, timeout)

static inline prism_err _v256_divN_ui32(const prdev_t* device, const ui8 type, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }

    return _prism_arch_send_opcode(device, PRISM_OPCODE_DIV_BASE + (vector_len % 8), type, timeout);
}
#define _v256_div8_ui32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_UI32, 8, timeout)
#define _v256_div7_ui32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_UI32, 7, timeout)
#define _v256_div6_ui32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_UI32, 6, timeout)
#define _v256_div5_ui32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_UI32, 5, timeout)
#define _v256_div4_ui32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_UI32, 4, timeout)
#define _v256_div3_ui32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_UI32, 3, timeout)
#define _v256_div2_ui32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_UI32, 2, timeout)
#define _v256_div1_ui32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_UI32, 1, timeout)
#define _v256_div8_si32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_SI32, 8, timeout)
#define _v256_div7_si32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_SI32, 7, timeout)
#define _v256_div6_si32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_SI32, 6, timeout)
#define _v256_div5_si32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_SI32, 5, timeout)
#define _v256_div4_si32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_SI32, 4, timeout)
#define _v256_div3_si32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_SI32, 3, timeout)
#define _v256_div2_si32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_SI32, 2, timeout)
#define _v256_div1_si32(device, timeout) _v256_divN_ui32(device, PRISM_OPCODE_TYPE_SI32, 1, timeout)

static inline prism_err _v256_andN_ui32(const prdev_t* device, const ui8 type, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }

    return _prism_arch_send_opcode(device, PRISM_OPCODE_AND_BASE + (vector_len % 8), type, timeout);
}
#define _v256_and8_ui32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_UI32, 8, timeout)
#define _v256_and7_ui32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_UI32, 7, timeout)
#define _v256_and6_ui32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_UI32, 6, timeout)
#define _v256_and5_ui32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_UI32, 5, timeout)
#define _v256_and4_ui32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_UI32, 4, timeout)
#define _v256_and3_ui32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_UI32, 3, timeout)
#define _v256_and2_ui32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_UI32, 2, timeout)
#define _v256_and1_ui32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_UI32, 1, timeout)
#define _v256_and8_si32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_SI32, 8, timeout)
#define _v256_and7_si32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_SI32, 7, timeout)
#define _v256_and6_si32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_SI32, 6, timeout)
#define _v256_and5_si32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_SI32, 5, timeout)
#define _v256_and4_si32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_SI32, 4, timeout)
#define _v256_and3_si32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_SI32, 3, timeout)
#define _v256_and2_si32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_SI32, 2, timeout)
#define _v256_and1_si32(device, timeout) _v256_andN_ui32(device, PRISM_OPCODE_TYPE_SI32, 1, timeout)

static inline prism_err _v256_nandN_ui32(const prdev_t* device, const ui8 type, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }

    return _prism_arch_send_opcode(device, PRISM_OPCODE_NAND_BASE + (vector_len % 8), type, timeout);
}

#define _v256_nand8_ui32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_UI32, 8, timeout)
#define _v256_nand7_ui32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_UI32, 7, timeout)
#define _v256_nand6_ui32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_UI32, 6, timeout)
#define _v256_nand5_ui32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_UI32, 5, timeout)
#define _v256_nand4_ui32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_UI32, 4, timeout)
#define _v256_nand3_ui32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_UI32, 3, timeout)
#define _v256_nand2_ui32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_UI32, 2, timeout)
#define _v256_nand1_ui32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_UI32, 1, timeout)
#define _v256_nand8_si32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_SI32, 8, timeout)
#define _v256_nand7_si32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_SI32, 7, timeout)
#define _v256_nand6_si32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_SI32, 6, timeout)
#define _v256_nand5_si32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_SI32, 5, timeout)
#define _v256_nand4_si32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_SI32, 4, timeout)
#define _v256_nand3_si32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_SI32, 3, timeout)
#define _v256_nand2_si32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_SI32, 2, timeout)
#define _v256_nand1_si32(device, timeout) _v256_nandN_ui32(device, PRISM_OPCODE_TYPE_SI32, 1, timeout)

static inline prism_err _v256_orN_ui32(const prdev_t* device, const ui8 type, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }

    return  _prism_arch_send_opcode(device, PRISM_OPCODE_OR_BASE + (vector_len % 8), type, timeout);
}
#define _v256_or8_ui32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_UI32, 8, timeout)
#define _v256_or7_ui32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_UI32, 7, timeout)
#define _v256_or6_ui32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_UI32, 6, timeout)
#define _v256_or5_ui32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_UI32, 5, timeout)
#define _v256_or4_ui32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_UI32, 4, timeout)
#define _v256_or3_ui32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_UI32, 3, timeout)
#define _v256_or2_ui32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_UI32, 2, timeout)
#define _v256_or1_ui32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_UI32, 1, timeout)
#define _v256_or8_si32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_SI32, 8, timeout)
#define _v256_or7_si32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_SI32, 7, timeout)
#define _v256_or6_si32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_SI32, 6, timeout)
#define _v256_or5_si32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_SI32, 5, timeout)
#define _v256_or4_si32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_SI32, 4, timeout)
#define _v256_or3_si32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_SI32, 3, timeout)
#define _v256_or2_si32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_SI32, 2, timeout)
#define _v256_or1_si32(device, timeout) _v256_orN_ui32(device, PRISM_OPCODE_TYPE_SI32, 1, timeout)

static inline prism_err _v256_xorN_ui32(const prdev_t* device, const ui8 type, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }

    return _prism_arch_send_opcode(device, PRISM_OPCODE_XOR_BASE + (vector_len % 8),type, timeout);
}
#define _v256_xor8_ui32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_UI32, 8, timeout)
#define _v256_xor7_ui32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_UI32, 7, timeout)
#define _v256_xor6_ui32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_UI32, 6, timeout)
#define _v256_xor5_ui32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_UI32, 5, timeout)
#define _v256_xor4_ui32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_UI32, 4, timeout)
#define _v256_xor3_ui32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_UI32, 3, timeout)
#define _v256_xor2_ui32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_UI32, 2, timeout)
#define _v256_xor1_ui32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_UI32, 1, timeout)
#define _v256_xor8_si32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_SI32, 8, timeout)
#define _v256_xor7_si32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_SI32, 7, timeout)
#define _v256_xor6_si32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_SI32, 6, timeout)
#define _v256_xor5_si32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_SI32, 5, timeout)
#define _v256_xor4_si32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_SI32, 4, timeout)
#define _v256_xor3_si32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_SI32, 3, timeout)
#define _v256_xor2_si32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_SI32, 2, timeout)
#define _v256_xor1_si32(device, timeout) _v256_xorN_ui32(device, PRISM_OPCODE_TYPE_SI32, 1, timeout)

static inline prism_err _v256_norN_ui32(const prdev_t* device, const ui8 type, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }

    return _prism_arch_send_opcode(device, PRISM_OPCODE_NOR_BASE + (vector_len % 8),type, timeout);
}

#define _v256_nor8_ui32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_UI32, 8, timeout)
#define _v256_nor7_ui32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_UI32, 7, timeout)
#define _v256_nor6_ui32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_UI32, 6, timeout)
#define _v256_nor5_ui32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_UI32, 5, timeout)
#define _v256_nor4_ui32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_UI32, 4, timeout)
#define _v256_nor3_ui32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_UI32, 3, timeout)
#define _v256_nor2_ui32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_UI32, 2, timeout)
#define _v256_nor1_ui32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_UI32, 1, timeout)
#define _v256_nor8_si32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_SI32, 8, timeout)
#define _v256_nor7_si32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_SI32, 7, timeout)
#define _v256_nor6_si32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_SI32, 6, timeout)
#define _v256_nor5_si32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_SI32, 5, timeout)
#define _v256_nor4_si32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_SI32, 4, timeout)
#define _v256_nor3_si32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_SI32, 3, timeout)
#define _v256_nor2_si32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_SI32, 2, timeout)
#define _v256_nor1_si32(device, timeout) _v256_norN_ui32(device, PRISM_OPCODE_TYPE_SI32, 1, timeout)

static inline prism_err _v256_notN_ui32(const prdev_t* device, const ui8 type, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }

    return _prism_arch_send_opcode(device, PRISM_OPCODE_NOT_BASE + (vector_len % 8), type, timeout);
}
#define _v256_not8_ui32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_UI32, 8, timeout)
#define _v256_not7_ui32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_UI32, 7, timeout)
#define _v256_not6_ui32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_UI32, 6, timeout)
#define _v256_not5_ui32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_UI32, 5, timeout)
#define _v256_not4_ui32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_UI32, 4, timeout)
#define _v256_not3_ui32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_UI32, 3, timeout)
#define _v256_not2_ui32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_UI32, 2, timeout)
#define _v256_not1_ui32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_UI32, 1, timeout)
#define _v256_not8_si32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_SI32, 8, timeout)
#define _v256_not7_si32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_SI32, 7, timeout)
#define _v256_not6_si32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_SI32, 6, timeout)
#define _v256_not5_si32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_SI32, 5, timeout)
#define _v256_not4_si32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_SI32, 4, timeout)
#define _v256_not3_si32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_SI32, 3, timeout)
#define _v256_not2_si32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_SI32, 2, timeout)
#define _v256_not1_si32(device, timeout) _v256_notN_ui32(device, PRISM_OPCODE_TYPE_SI32, 1, timeout)

static inline prism_err _v256_notc_x(const prdev_t* device, const ui8 type, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }

    return _prism_arch_send_opcode(device, PRISM_OPCODE_NOTC, type, timeout);
}

#define _v256_notc8_ui32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_UI32, 8, timeout)
#define _v256_notc7_ui32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_UI32, 7, timeout)
#define _v256_notc6_ui32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_UI32, 6, timeout)
#define _v256_notc5_ui32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_UI32, 5, timeout)
#define _v256_notc4_ui32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_UI32, 4, timeout)
#define _v256_notc3_ui32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_UI32, 3, timeout)
#define _v256_notc2_ui32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_UI32, 2, timeout)
#define _v256_notc1_ui32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_UI32, 1, timeout)
#define _v256_notc8_si32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_SI32, 8, timeout)
#define _v256_notc7_si32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_SI32, 7, timeout)
#define _v256_notc6_si32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_SI32, 6, timeout)
#define _v256_notc5_si32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_SI32, 5, timeout)
#define _v256_notc4_si32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_SI32, 4, timeout)
#define _v256_notc3_si32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_SI32, 3, timeout)
#define _v256_notc2_si32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_SI32, 2, timeout)
#define _v256_notc1_si32(device, timeout) _v256_notc_x(device, PRISM_OPCODE_TYPE_SI32, 1, timeout)

static inline prism_err _v256_cpm_epN_px(const prdev_t* device, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }
    
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CMP_EQ, PRISM_OPCODE_TYPE_UI32, timeout);
}
#define _v256_cpm_ep8_px(device, timeout) _v256_cpm_epN_px(device, 8, timeout)
#define _v256_cpm_ep7_px(device, timeout) _v256_cpm_epN_px(device, 7, timeout)
#define _v256_cpm_ep6_px(device, timeout) _v256_cpm_epN_px(device, 6, timeout)
#define _v256_cpm_ep5_px(device, timeout) _v256_cpm_epN_px(device, 5, timeout)
#define _v256_cpm_ep4_px(device, timeout) _v256_cpm_epN_px(device, 4, timeout)
#define _v256_cpm_ep3_px(device, timeout) _v256_cpm_epN_px(device, 3, timeout)
#define _v256_cpm_ep2_px(device, timeout) _v256_cpm_epN_px(device, 2, timeout)
#define _v256_cpm_ep1_px(device, timeout) _v256_cpm_epN_px(device, 1, timeout)

static inline prism_err _v256_cpm_nepN_px(const prdev_t* device, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }
    
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CMP_NE, PRISM_OPCODE_TYPE_UI32, timeout);
}

#define _v256_cpm_nep8_px(device, timeout) _v256_cpm_nepN_px(device, 8, timeout)
#define _v256_cpm_nep7_px(device, timeout) _v256_cpm_nepN_px(device, 7, timeout)
#define _v256_cpm_nep6_px(device, timeout) _v256_cpm_nepN_px(device, 6, timeout)
#define _v256_cpm_nep5_px(device, timeout) _v256_cpm_nepN_px(device, 5, timeout)
#define _v256_cpm_nep4_px(device, timeout) _v256_cpm_nepN_px(device, 4, timeout)
#define _v256_cpm_nep3_px(device, timeout) _v256_cpm_nepN_px(device, 3, timeout)
#define _v256_cpm_nep2_px(device, timeout) _v256_cpm_nepN_px(device, 2, timeout)
#define _v256_cpm_nep1_px(device, timeout) _v256_cpm_nepN_px(device, 1, timeout)

static inline prism_err _v256_cpm_gt_px(const prdev_t* device, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }
    
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CMP_GT, PRISM_OPCODE_TYPE_UI32, timeout);
}

#define _v256_cpm_gt8_px(device, timeout) _v256_cpm_gt_px(device, 8, timeout)
#define _v256_cpm_gt7_px(device, timeout) _v256_cpm_gt_px(device, 7, timeout)
#define _v256_cpm_gt6_px(device, timeout) _v256_cpm_gt_px(device, 6, timeout)
#define _v256_cpm_gt5_px(device, timeout) _v256_cpm_gt_px(device, 5, timeout)
#define _v256_cpm_gt4_px(device, timeout) _v256_cpm_gt_px(device, 4, timeout)
#define _v256_cpm_gt3_px(device, timeout) _v256_cpm_gt_px(device, 3, timeout)
#define _v256_cpm_gt2_px(device, timeout) _v256_cpm_gt_px(device, 2, timeout)
#define _v256_cpm_gt1_px(device, timeout) _v256_cpm_gt_px(device, 1, timeout)

static inline prism_err _v256_cpm_geN_px(const prdev_t* device, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }
    
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CMP_GE, PRISM_OPCODE_TYPE_UI32, timeout);
}

#define _v256_cpm_ge8_px(device, timeout) _v256_cpm_geN_px(device, 8, timeout)
#define _v256_cpm_ge7_px(device, timeout) _v256_cpm_geN_px(device, 7, timeout)
#define _v256_cpm_ge6_px(device, timeout) _v256_cpm_geN_px(device, 6, timeout)
#define _v256_cpm_ge5_px(device, timeout) _v256_cpm_geN_px(device, 5, timeout)
#define _v256_cpm_ge4_px(device, timeout) _v256_cpm_geN_px(device, 4, timeout)
#define _v256_cpm_ge3_px(device, timeout) _v256_cpm_geN_px(device, 3, timeout)
#define _v256_cpm_ge2_px(device, timeout) _v256_cpm_geN_px(device, 2, timeout)
#define _v256_cpm_ge1_px(device, timeout) _v256_cpm_geN_px(device, 1, timeout)

static inline prism_err _v256_cpm_ltN_px(const prdev_t* device, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }
    
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CMP_LT, PRISM_OPCODE_TYPE_UI32, timeout);
}

#define _v256_cpm_lt8_px(device, timeout) _v256_cpm_ltN_px(device, 8, timeout)
#define _v256_cpm_lt7_px(device, timeout) _v256_cpm_ltN_px(device, 7, timeout)
#define _v256_cpm_lt6_px(device, timeout) _v256_cpm_ltN_px(device, 6, timeout)
#define _v256_cpm_lt5_px(device, timeout) _v256_cpm_ltN_px(device, 5, timeout)
#define _v256_cpm_lt4_px(device, timeout) _v256_cpm_ltN_px(device, 4, timeout)
#define _v256_cpm_lt3_px(device, timeout) _v256_cpm_ltN_px(device, 3, timeout)
#define _v256_cpm_lt2_px(device, timeout) _v256_cpm_ltN_px(device, 2, timeout)
#define _v256_cpm_lt1_px(device, timeout) _v256_cpm_ltN_px(device, 1, timeout)

static inline prism_err _v256_cpm_leN_px(const prdev_t* device, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }
    
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CMP_LE, PRISM_OPCODE_TYPE_UI32, timeout);
}

#define _v256_cpm_le8_px(device, timeout) _v256_cpm_leN_px(device, 8, timeout)
#define _v256_cpm_le7_px(device, timeout) _v256_cpm_leN_px(device, 7, timeout)
#define _v256_cpm_le6_px(device, timeout) _v256_cpm_leN_px(device, 6, timeout)
#define _v256_cpm_le5_px(device, timeout) _v256_cpm_leN_px(device, 5, timeout)
#define _v256_cpm_le4_px(device, timeout) _v256_cpm_leN_px(device, 4, timeout)
#define _v256_cpm_le3_px(device, timeout) _v256_cpm_leN_px(device, 3, timeout)
#define _v256_cpm_le2_px(device, timeout) _v256_cpm_leN_px(device, 2, timeout)
#define _v256_cpm_le1_px(device, timeout) _v256_cpm_leN_px(device, 1, timeout)

static inline prism_err _v256_cpm_cplN_px(const prdev_t* device, const ui8 vector_len, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }
    
    return _prism_arch_send_opcode(device, PRISM_OPCODE_CPL2, PRISM_OPCODE_TYPE_UI32, timeout);
}
#define _v256_cpm_cpl8_px(device, timeout) _v256_cpm_cplN_px(device, 8, timeout)
#define _v256_cpm_cpl7_px(device, timeout) _v256_cpm_cplN_px(device, 7, timeout)
#define _v256_cpm_cpl6_px(device, timeout) _v256_cpm_cplN_px(device, 6, timeout)
#define _v256_cpm_cpl5_px(device, timeout) _v256_cpm_cplN_px(device, 5, timeout)
#define _v256_cpm_cpl4_px(device, timeout) _v256_cpm_cplN_px(device, 4, timeout)
#define _v256_cpm_cpl3_px(device, timeout) _v256_cpm_cplN_px(device, 3, timeout)
#define _v256_cpm_cpl2_px(device, timeout) _v256_cpm_cplN_px(device, 2, timeout)
#define _v256_cpm_cpl1_px(device, timeout) _v256_cpm_cplN_px(device, 1, timeout)

static inline prism_err _v256_cpm_shlN_pa(const prdev_t* device, const ui8 vector_len, const ui8 num, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }
    
    return _prism_arch_send_opcode_arg1(device, PRISM_OPCODE_SHIFT_L, PRISM_OPCODE_TYPE_UI32, (void*)&num, timeout);
}

#define _v256_cpm_shl8_pa(device, num, timeout) _v256_cpm_shlN_pa(device, 8, num, timeout)
#define _v256_cpm_shl7_pa(device, num, timeout) _v256_cpm_shlN_pa(device, 7, num, timeout)
#define _v256_cpm_shl6_pa(device, num, timeout) _v256_cpm_shlN_pa(device, 6, num, timeout)
#define _v256_cpm_shl5_pa(device, num, timeout) _v256_cpm_shlN_pa(device, 5, num, timeout)
#define _v256_cpm_shl4_pa(device, num, timeout) _v256_cpm_shlN_pa(device, 4, num, timeout)
#define _v256_cpm_shl3_pa(device, num, timeout) _v256_cpm_shlN_pa(device, 3, num, timeout)
#define _v256_cpm_shl2_pa(device, num, timeout) _v256_cpm_shlN_pa(device, 2, num, timeout)
#define _v256_cpm_shl1_pa(device, num, timeout) _v256_cpm_shlN_pa(device, 1, num, timeout)

static inline prism_err _v256_cpm_shrN_pa(const prdev_t* device, const ui8 vector_len, const ui8 num, ui32 timeout) {
    if (device == 0) { return PR_ERR_INVALID_ARGUMENT; }
    if (vector_len < 1 || vector_len > 8) { return PR_ERR_INVALID_ARGUMENT;  }
    
    return _prism_arch_send_opcode_arg1(device, PRISM_OPCODE_SHIFT_R, PRISM_OPCODE_TYPE_UI32, (void*)&num, timeout);
}

#define _v256_cpm_shr8_pa(device, num, timeout) _v256_cpm_shrN_pa(device, 8, num, timeout)
#define _v256_cpm_shr7_pa(device, num, timeout) _v256_cpm_shrN_pa(device, 7, num, timeout)
#define _v256_cpm_shr6_pa(device, num, timeout) _v256_cpm_shrN_pa(device, 6, num, timeout)
#define _v256_cpm_shr5_pa(device, num, timeout) _v256_cpm_shrN_pa(device, 5, num, timeout)
#define _v256_cpm_shr4_pa(device, num, timeout) _v256_cpm_shrN_pa(device, 4, num, timeout)
#define _v256_cpm_shr3_pa(device, num, timeout) _v256_cpm_shrN_pa(device, 3, num, timeout)
#define _v256_cpm_shr2_pa(device, num, timeout) _v256_cpm_shrN_pa(device, 2, num, timeout)
#define _v256_cpm_shr1_pa(device, num, timeout) _v256_cpm_shrN_pa(device, 1, num, timeout)

#if __cplusplus
}
#endif // __cplusplus

#endif // __PRISM_H__
