/*PGR-GNU*****************************************************************

File: basePath_SSEC.hpp
Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

#pragma once

#include <deque>
#include <iostream>
#include <algorithm>
#include "postgres.h"
#include "./pgr_types.h"

class Path {
    typedef std::deque< Path_t >::iterator pthIt;
    typedef std::deque< Path_t >::const_iterator ConstpthIt;
 private:
    std::deque< Path_t > path;
    int64_t m_start_id;
    int64_t m_end_id;
    double m_tot_cost;

 public:
    Path(): m_tot_cost(0) {}
    Path(int64_t s_id, int64_t e_id)
        : m_start_id(s_id), m_end_id(e_id), m_tot_cost(0)
    {}
    int64_t start_id() const {return m_start_id;}
    void start_id(int64_t value) {m_start_id = value;}
    int64_t end_id()  const {return m_end_id;}
    void end_id(int64_t value) {m_end_id = value;}
    int64_t tot_cost()  const {return m_tot_cost;}

    size_t size() const {return path.size();}
    bool empty() const {return path.empty();}

    void push_front(Path_t data);
    void push_back(Path_t data);
    const Path_t& operator[](size_t i) const {return path[i];}
    Path_t& operator[](size_t i) {return path[i];}
    pthIt begin() {return path.begin();}
    pthIt end() {return path.end();}
    void erase(pthIt pos) {path.erase(pos);}
    ConstpthIt begin() const {return path.begin();}
    ConstpthIt end() const {return path.end();}


    Path_t set_data(
            int64_t d_from, 
            int64_t d_to,
            int64_t d_vertex,
            int64_t d_edge, 
            float8 d_cost,
            float8 d_tot_cost);

    void push_front(
            int64_t d_vertex,
            int64_t d_edge, 
            float8 d_cost,
            float8 d_tot_cost);
    void clear();

    void print_path(std::ostream &log) const;
    void print_path() const;


    void fix_path(int64_t from, int64_t to);


    Path  getSubpath(unsigned int j) const;


    bool isEqual(const Path &subpath) const;
    void appendPath(const Path &o_path);
    void empty_path(unsigned int d_vertex);

    void get_pg_dd_path(
            General_path_element_t **ret_path,
            int &sequence) const;

    void get_pg_ksp_path(
            General_path_element_t **ret_path,
            int &sequence, int routeId) const;

    void generate_postgres_data(
            General_path_element_t **postgres_data,
            size_t &sequence) const;

    friend size_t collapse_paths(
            General_path_element_t **ret_path,
            const std::deque< Path > &paths) {
        size_t sequence = 0;
        for (const Path &path : paths) {
            if (path.path.size() > 0)
                path.generate_postgres_data(ret_path, sequence);
        }
        return sequence;
    }



    /*
     * sort the paths by size from greater to smaller
     *        and sort each path by node 
     * all the nodes on p2 are going to be compared
     * with the nodes of p1
     *
     * When both paths reach the node and p1.agg_cost > p2.agg_cost
     *    erase the node of p1 
     *    (cant erase from p2 because we loose the iterators
     *     so in a future cycle it will be deleted)
     *
     * sort the paths by start_id, 
     */

    friend void equi_cost(std::deque< Path > &paths) {

        /* sort paths by size */
        std::sort(paths.begin(), paths.end(), 
                [](const Path &e1, const Path &e2)->bool { 
                return e1.size() > e2.size(); 
                });

        /* sort each path by node */
        for (auto &p : paths) {
            std::sort(p.begin(), p.end(), 
                    [](const Path_t &e1, const Path_t &e2)->bool { 
                    return e1.node < e2.node; 
                    });
        }

        for (auto &p1 : paths) {
            for (const auto &p2 : paths) {
                if (p1.start_id() == p2.start_id()) continue;
                for (const auto &stop : p2.path) {
                    /* find the node of p2 in p1 */
                    auto pos = find_if(p1.begin(), p1.end(),
                            [&stop](const Path_t &stop1)->bool { 
                            return stop.node == stop1.node; 
                            });
                    if (pos != p1.end() && stop.agg_cost < pos->agg_cost) {
                        /* both share the same node &
                         * the second path has the smallest
                         *  So erasing from the first path */
                        p1.erase(pos);
                    }
                }
            }
        }

        /* sort paths by start_id */
        std::sort(paths.begin(), paths.end(), 
                [](const Path &e1, const Path &e2)->bool { 
                return e1.start_id() < e2.start_id(); 
                });

        /* sort each path by agg_cost, node */
        for (auto &path : paths) {
            /* order by agg_cost , edge */
            std::sort(path.begin(), path.end(),
                    [](const Path_t &l, const  Path_t &r)   
                    { return l.agg_cost < r.agg_cost? true : l.node < r.node;});
        }                               

    }

    friend int count_tuples(const std::deque< Path > &paths) {
        int count(0);
        for (const Path &e : paths) {
            count += e.path.size();
        }
        return count;
    }
};

