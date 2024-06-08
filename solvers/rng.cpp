// stolen from https://atcoder.jp/contests/ahc031/submissions/51940007

#ifndef RNG_CPP
#define RNG_CPP

class Rng {
public:
    // to avoid bugs
    Rng(const Rng&) = delete;

    Rng& operator=(const Rng&&) = delete;

    Rng(int s) {
        seed(s);
    }

    void seed(int s) {
        state = s + INCR;
        next32();
    }

    int next_int() {
        return next31();
    }

    int next_int(int mod) {
        assert(mod > 0);
        return (int) ((unsigned long long) next31() * mod >> 31);
    }

    int next_int(int min, int max) {
        return min + next_int(max - min + 1);
    }

    unsigned int next_uint() {
        return next32();
    }

    unsigned long long next_ull() {
        return (unsigned long long) next32() << 32 | next32();
    }

    double next_double() {
        return (double) next31() / 0x80000000;
    }

    double next_double(double min, double max) {
        return min + next_double() * (max - min);
    }

    double next_normal() {
        return sqrt(-2 * log(next_double())) * cos(6.283185307179586 * next_double());
    }

    double next_normal(double mean, double sigma) {
        return mean + next_normal() * sigma;
    }
private:
    static constexpr unsigned long long MULT = 0x8b46ad15ae59daadull;
    static constexpr unsigned long long INCR = 0xf51827be20401689ull;
    unsigned long long state;

    unsigned int next32() {
        unsigned int r = (unsigned int) (state >> 59);
        state = state * MULT + INCR;
        state ^= state >> 18;
        unsigned int t = (unsigned int) (state >> 27);
        return t >> r | t << (-r & 31);
    }

    int next31() {
        return (int) (next32() & 0x7fffffff);
    }
};

#endif // RNG_CPP
