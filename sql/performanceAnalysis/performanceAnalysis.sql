/*PGR-GNU*****************************************************************
File: performanceAnalysis.sql

Generated with Template by:
Copyright (c) 2016 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2017 Rohith Reddy
Mail: rohithreddy2219.com

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

CREATE OR REPLACE FUNCTION pgr_performanceAnalysis(
    edges_sql TEXT,
    algorithm TEXT,
    start_vids ANYARRAY,
    end_vids ANYARRAY,
    level INTEGER DEFAULT 10,
    num_iterations INTEGER DEFAULT 1, 
    directed BOOLEAN DEFAULT true,
    only_cost BOOLEAN DEFAULT false,
    OUT seq INTEGER,
    OUT source BIGINT,
    OUT target BIGINT,
    OUT level INTEGER,
    OUT num_edges BIGINT,
    OUT num_vertices BIGINT,
    OUT build_time FLOAT,
    OUT computation_time FLOAT,
    OUT path_len FLOAT)

RETURNS SETOF RECORD AS
'$libdir/${PGROUTING_LIBRARY_NAME}', 'performanceAnalysis'
LANGUAGE c IMMUTABLE STRICT;

