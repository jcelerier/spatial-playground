#pragma once
#include <ginac/ginac.h>
#include <QDebug>
class address{};

class space
{
    public:
        using variable_lst = std::vector<GiNaC::symbol>;

        space(const variable_lst& vars):
            m_variables(vars)
        {

        }

        const variable_lst& variables() const
        { return m_variables; }

    private:
        variable_lst m_variables;
};

class area
{
    public:
        using variable_lst = std::vector<GiNaC::symbol>;
        using parameter = GiNaC::symbol;
        using number = GiNaC::numeric;
        using parameter_map = GiNaC::exmap;
        using variable_map = GiNaC::exmap;
        using numlst = GiNaC::lst;
        using equation = GiNaC::relational;

        // Symbol1 : x, y
        // Symbol2 : p_0, p_1 : the parameters

        // ex: (x - p_0)² + (y - p_1)² <= p_2² for a disc
        area(const equation& e,
             const variable_lst& vars,
             const parameter_map& params):
            m_exp{e},
            m_variables(vars),
            m_parameters(params)
        {

        }

        // Set a parameter
        void set(const parameter& sym, const number& val)
        {
            m_parameters.at(sym) = val;
        }

        equation map_to_space(const space& s) const
        {
            // Space dim must be >= to variables dim
            Q_ASSERT(s.variables().size() >= m_variables.size());

            GiNaC::exmap m;
            for(int i = 0; i < m_variables.size(); i++)
            {
                m.insert({m_variables[i], s.variables()[i]});
            }

            return GiNaC::ex_to<equation>(m_exp.subs(m));
        }

        bool has(const equation& e, const variable_map& vars) const
        {
            GiNaC::exmap m = m_parameters;
            m.insert(vars.begin(), vars.end());

            return bool(GiNaC::ex_to<equation>(e.subs(m)));
        }

        const variable_lst& variables() const
        { return m_variables; }

        const parameter_map& parameters() const
        { return m_parameters; }

    private:
        equation m_exp;
        variable_lst m_variables;

        // Map between symbol and numeric value.
        parameter_map m_parameters;
};

template<typename Size, typename Device>
class square_renderer
{
    public:
        Device render_device;
        double side = 5.; // square size
        Size size; //complete size.

        // How to make this dimension-agnostic ?
        void render(const area& a, const space& s)
        {
            GiNaC::exmap var_map;
            auto eqn = a.map_to_space(s);

            for(int x = 0; x < size.x(); x += side)
            {
                var_map[s.variables()[0]] = x;
                for(int y = 0; y < size.y(); y += side)
                {
                    var_map[s.variables()[1]] = y;

                    if(a.has(eqn, var_map))
                    {
                        render_device.add(x-side/2., y-side/2., side, side);
                    }
                }
            }
        }
};
