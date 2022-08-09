// graph_framework.h : Include file for standard system include files,
// or project specific include files.

// TODO!!!!!!!!!!
// ALLOW ACCESS OF DATA FOR USER potialt pointer to vector(or to data_t) instead of vector of point
// HANDLE EXCEPTIONS
// Potential sorting of edges to increase contruction speed (ESPECIALLY the find EOG)
// review of data structures
// decontrution of graphs
// changing graphs around
// add optimization for EoG choices
// add mutliple start edges
// maybe use data ptr as identify, and bool as flag
#pragma once


#include "graph.h"

#include "graph_end_node.h"
#include "start_node.h"
#include "dynamic_join_node.h"
#include "combine_node.h"
#include "filter_node.h"
#include "processor_node.h"

