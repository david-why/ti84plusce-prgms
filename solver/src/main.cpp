#include <stdarg.h>
#include <tice.h>

#include <TINYSTL/vector.h>

#include "menu.cpp"
#include "solvers.cpp"

char buffer[512];

int main()
{
    os_ClrHome();
    initSolvers();

    while (true)
    {
        input_t input;
        tinystl::vector<uint8_t> values[UNIT_TYPE_MAX];

        bool cont = false;
        while (true)
        {
            uint8_t type = menu("Unit", unit_type_names, UNIT_TYPE_MAX);
            if (type == 0)
                break;
            float val = numericInput("Value: ", buffer, 512);
            input.types[type - 1]++;
            input.values[type - 1].push_back(val);
            cont = true;
        }
        if (!cont)
            break;

        const tinystl::vector<solver_t *> &solvers = findSolver(input);
        if (solvers.size() == 0)
        {
            os_PutStrFull("No solver matches input.");
            while (!os_GetCSC())
                ;
            continue;
        }
        solver_t *solver;
        if (solvers.size() == 1)
            solver = solvers[0];
        else
        {
            const char *names[solvers.size()];
            for (uint8_t i = 0; i < solvers.size(); i++)
                names[i] = solvers[i]->name;
            uint8_t index = menu("Choose solver", names, solvers.size());
            if (index == 0)
                continue;
            solver = solvers[index - 1];
        }
        os_ClrHome();
        float ans = solver->func(input);
        real_t r = os_FloatToReal(ans);
        os_RealToStr(buffer, &r, 127, 1, -1);
        os_PutStrLine("The answer is:");
        outchar('\n');
        outchar('\n');
        os_PutStrLine(buffer);
        os_PutStrLine(" (");
        os_PutStrLine(unit_type_names[solver->output]);
        os_PutStrLine(")");
        outchar('\n');
        outchar('\n');
        os_PutStrLine("Brought to you by solver:");
        outchar('\n');
        os_PutStrLine(solver->name);
        while (!os_GetCSC())
            ;
    }

    return 0;
}
