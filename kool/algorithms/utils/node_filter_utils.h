/** Copyright : Arthur Bit-Monnot (2013)  arthur.bit-monnot@laas.fr

This software is a computer program whose purpose is to [describe
functionalities and technical features of your software].

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms. 
*/

#ifndef NODE_FILTER_UTILS_H
#define NODE_FILTER_UTILS_H


#include "nodes_filter.h"
#include "reglc_graph.h"

/**
 * Builds an isochrone filter of maximum cost 'max_time' around 'center'
 */
NodeSet * isochrone ( const RLC::AbstractGraph * g, const int center, const int max_time );

struct my_isochrone{
  NodeSet *ns;
  std::vector<int> costs;
};

my_isochrone cool_iso( const RLC::AbstractGraph * g, const int center, const int max_time );

VisualResult show_isochrone ( const RLC::AbstractGraph * g, const int center, const int max_time );


BBNodeFilter * rectangle_containing ( const Transport::Graph * trans, const int node1, const int node2, const float margin );
BBNodeFilter * rectangle_containing ( const Transport::Graph* trans, const std::vector<int> nodes, const float margin );

BBNodeFilter * bordeaux_bb ( const Transport::Graph * trans );

BBNodeFilter * toulouse_bb ( const Transport::Graph * trans );



#endif
