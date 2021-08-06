#include <stdarg.h>
#include <string.h>
#include <tice.h>
#include <TINYSTL/vector.h>

typedef enum
{
    time,
    rate,
    distance,
    acceleration,
    UNIT_TYPE_MAX
} unit_type_t;

const char *unit_type_names[] = {"Time", "Rate", "Distance", "Acceleration"};

typedef struct input_t
{
    uint8_t types[UNIT_TYPE_MAX] = {0};
    tinystl::vector<float> values[UNIT_TYPE_MAX];

    bool operator==(const input_t &other)
    {
        for (uint8_t i = 0; i < UNIT_TYPE_MAX; i++)
            if (types[i] != other.types[i])
                return false;
        return true;
    }

    input_t() {}
    input_t(uint8_t t[]) { memcpy(types, t, UNIT_TYPE_MAX); }
} input_t;

typedef struct
{
    input_t inputs;
    unit_type_t output;
    float (*func)(input_t &);
    const char *name;
} solver_t;

tinystl::vector<solver_t> solvers;

tinystl::vector<solver_t *> findSolver(const input_t &inputs)
{
    tinystl::vector<solver_t *> ret;
    for (tinystl::vector<solver_t>::iterator it = solvers.begin(); it != solvers.end(); it++)
        if ((*it).inputs == inputs)
            ret.push_back(it);
    return ret;
}

bool registerSolver(const input_t &inputs, unit_type_t output, float (*func)(input_t &), const char *name)
{
    for (tinystl::vector<solver_t>::iterator it = solvers.begin(); it != solvers.end(); it++)
        if ((*it).func == func)
            return false;
    solver_t solver = {inputs, output, func, name};
    solvers.push_back(solver);
    return true;
}

#define SLVER(name) float name(input_t &inputs)
#define TIME(n) inputs.values[time][n]
#define RATE(n) inputs.values[rate][n]
#define DIST(n) inputs.values[distance][n]
#define ACC(n) inputs.values[acceleration][n]

// --- SOLVERS ---

SLVER(motion1) { return RATE(0) * TIME(0); }
SLVER(motion2) { return DIST(0) / TIME(0); }
SLVER(motion3) { return DIST(0) / RATE(0); }
SLVER(acc1) { return ACC(0) * TIME(0); }

// --- END SOLVERS ---

#define LST(__VA_ARGS__...) {(uint8_t[]){__VA_ARGS__}}

void initSolvers()
{
    registerSolver(LST(1, 1, 0, 0), distance, motion1, "D=RT");
    registerSolver(LST(1, 0, 1, 0), rate, motion2, "R=D/T");
    registerSolver(LST(0, 1, 1, 0), time, motion3, "T=D/R");
    registerSolver(LST(1, 0, 0, 1), rate, acc1, "V=AT");
}
