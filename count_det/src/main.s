.text

# a11·a22·a33 + a12·a23·a31 + a13·a21·a32 - a13·a22·a31 - a11·a23·a32 - a12·a21·a33
.global count_det
count_det:

    eor r1, r1;

# + n[0] * n[4] * n[8]
    ldr r2, [r0, #0]
    ldr r3, [r0, #16]
    mul r2, r3
    ldr r3, [r0, #32]
    mla r1, r2, r3, r1

# + n[1] * n[5] * n[6]
    ldr r2, [r0, #4]
    ldr r3, [r0, #20]
    mul r2, r3
    ldr r3, [r0, #24]
    mla r1, r2, r3, r1

# + n[2] * n[3] * n[7]
    ldr r2, [r0, #8]
    ldr r3, [r0, #12]
    mul r2, r3
    ldr r3, [r0, #28]
    mla r1, r2, r3, r1

# - n[2] * n[4] * n[6]
    ldr r2, [r0, #8]
    ldr r3, [r0, #16]
    mul r2, r3
    ldr r3, [r0, #24]
    mls r1, r2, r3, r1

# - n[0] * n[5] * n[7]
    ldr r2, [r0, #0]
    ldr r3, [r0, #20]
    mul r2, r3
    ldr r3, [r0, #28]
    mls r1, r2, r3, r1

# - n[1] * n[3] * n[8]
    ldr r2, [r0, #4]
    ldr r3, [r0, #12]
    mul r2, r3
    ldr r3, [r0, #32]
    mls r1, r2, r3, r1

    mov r0, r1
    bx lr
