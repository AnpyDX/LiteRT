
uint gen_seed(vec2 pix, uint width, uint height, uint counter) {
    return (uint(
            uint((pix.x * 0.5 + 0.5) * width)  * uint(1973) + 
            uint((pix.y * 0.5 + 0.5) * height) * uint(9277) + 
            uint(counter) * uint(26699)) 
        | uint(1));
}

uint wang_hash(inout uint seed) {
    seed = uint(seed ^ uint(61)) ^ uint(seed >> uint(16));
    seed *= uint(9);
    seed = seed ^ (seed >> 4);
    seed *= uint(0x27d4eb2d);
    seed = seed ^ (seed >> 15);
    return seed;
}
 
float rand_0_to_1(uint seed) {
    return float(wang_hash(seed)) / 4294967296.0;
}