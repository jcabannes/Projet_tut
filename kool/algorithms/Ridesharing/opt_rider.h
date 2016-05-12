#pragma once
#include "header.h"
#include "aux_data.h"

/*
 * Model for Pedestrian
 */

using namespace MuPaRo;

namespace Privacy {
  class Pedestrian{
    public:
    
   
    int posStart, posEnd;
    database data_before, data_after, dBefore, dAfter;
    prefs favorites;
    
    Pedestrian(){
      
    }
    Pedestrian(int start, int end){
      
      posStart=start;
      posEnd=end; 
    }    
    ~Pedestrian(){}

    journey getMyJourney(const Transport::Graph * g){

      journey jr;

      Node start=g->mapNode(posStart);
      Node end=g->mapNode(posEnd);

      jr.origin.lat=start.lat;
      jr.origin.lon=start.lon;

      jr.destination.lat=end.lat;
      jr.destination.lon=end.lon;

      return jr;
    }

    /*
    * return node without cost
    */
    BiList getNodeOnly(){
      
      BiList bl;
      
      BOOST_FOREACH(auto& mpair, data_before){
        data d = mpair.second;
        bl.PickUp.push_back(d.node);
      } 

      BOOST_FOREACH(auto& mpair, data_after){
        data t = mpair.second;
        bl.DropOff.push_back(t.node);
      } 
  	
      return bl;
    }
    
    /*
      * return pickup node without cost
      */  
    NodeList getPickup(){
      NodeList nl;
      BOOST_FOREACH(auto& mpair, data_before){
        data d = mpair.second;
        nl.push_back(d.node);
      }
      return nl;
    }
    NodeList getPickup(int limit){
      NodeList nl;
      BOOST_FOREACH(auto& mpair, data_before){
        data d = mpair.second;
        if(d.cost<=limit){
        nl.push_back(d.node);
        }
      }
      return nl;
    }
    
    NodeList getPickup(int limit_a, int limit_b){
      NodeList nl;
      BOOST_FOREACH(auto& mpair, data_before){
        data d = mpair.second;
        if(d.cost<=limit_b && d.cost>limit_a){
        nl.push_back(d.node);
        }
      }
      return nl;
    }
    /*
      * return dropoff node without cost
      */  
    NodeList getDropoff(){
      NodeList nl;
      BOOST_FOREACH(auto& mpair, data_after){
        data d = mpair.second;
        nl.push_back(d.node);
      }
      return nl;
    }
    
     NodeList getDropoff(int limit){
      NodeList nl;
      BOOST_FOREACH(auto& mpair, data_after){
        data d = mpair.second;
        if(d.cost<=limit){
        nl.push_back(d.node);
        }
      }
      return nl;
    }
    
     NodeList getDropoff(int limit_a, int limit_b){
      NodeList nl;
      BOOST_FOREACH(auto& mpair, data_after){
        data d = mpair.second;
        if(d.cost<=limit_b && d.cost>limit_a){
        nl.push_back(d.node);
        }
      }
      return nl;
    }
      
    
    
    /*
      *computes isochrones to search for potential pickup
      */ 
    void findPickup(const Transport::Graph * g, int limit_a){
      
      dBefore.clear();
      RLC::Graph *ag=new RLC::Graph( g,dfa_passenger );
      my_isochrone m1=cool_iso (ag, posStart, limit_a);

      for(int i=0; i<g->num_vertices(); i++){
        Node k=g->mapNode(i);
        
        if (m1.ns->bitset[i] && k.pick_up){
        	data db;
        	db.node=i;
        	db.cost=m1.costs[i];
        	dBefore[i]=db;
        }
      }
    }

    void choosePickup(int limit_a, int limit_b){

      data_before.clear();
     
      BOOST_FOREACH(auto& mpair, dBefore){
        data d = mpair.second;
        if(d.cost<=limit_b && d.cost>=limit_a){
        data_before[d.node]=d;
        }
      }

    }
    
    void findDropoff(const Transport::Graph * g, int limit_b){
      
      
      dAfter.clear();
      
      RLC::Graph *ag=new RLC::Graph( g,dfa_passenger );
      my_isochrone m2=cool_iso (ag, posEnd, limit_b);

      for(int i=0; i<g->num_vertices(); i++){
        Node k=g->mapNode(i);
        
        if (m2.ns->bitset[i] && k.pick_up){
        	data da;
        	da.node=i;
        	da.cost=m2.costs[i];
        	dAfter[i]=da;
        } 
      }

    }

    void chooseDropoff(int limit_a, int limit_b){

      data_after.clear();
     
      BOOST_FOREACH(auto& mpair, dAfter){
        data d = mpair.second;
        if(d.cost<=limit_b && d.cost>=limit_a){
        data_after[d.node]=d;
        }
      }

    }
    
        
    /*
    * return the total traveling duration
    * */
    costVector ProcessTotalCost(pathList paths){
        
      costVector cv;

      BOOST_FOREACH(path p, paths){
        data d1=getData2(p.start, data_before), d2=getData2(p.end, data_after);
        totalCost tc;
        tc.path_id=p.id;
        tc.cost=d1.cost+p.cost+d2.cost;
        cv.push_back(tc);
      }
      
      return cv;
    }

    /*
    * return the total traveling duration with double cost
    * */
    /*costVector ProcessTotalCost_with_doubleCost(pathList paths){
        
      costVector cv;

      BOOST_FOREACH(path p, paths){
        data d1=getData(p.start, data_before), d2=getData(p.end, data_after);
        totalCost tc;
        tc.path_id=p.id;
        tc.cost=d1.cost+p.cost+d2.cost;
        cv.push_back(tc);
      }
      
      return cv;
    }*/

    /*
    * order favorites path according to the total traveling time
    * */
    void getFavorites(pathList paths){
  	  
      costVector cv=ProcessTotalCost(paths);
      cv.sort(compare_cost);
      int n=cv.size();
      
      BOOST_FOREACH(totalCost tc, cv){
      	pref p;
      	p.id=tc.path_id;
      	p.weight=n;
      	n--;
      	favorites.push_back(p);
      }	
    }

     /*costVector ProcessTotalCostB(pathList paths){
        
      costVector cv;

      BOOST_FOREACH(path p, paths){
        data d1=getData(p.start, data_before), d2=getData(p.end, data_after);
        totalCost tc;
        tc.path_id=p.id;
        tc.cost=2*d1.cost+p.cost+d2.cost;
        cv.push_back(tc);
      }
      
      return cv;
    }


    void getFavoritesB(pathList paths){
      
      costVector cv=ProcessTotalCostB(paths);
      cv.sort(compare_cost);
      int n=cv.size();
      
      BOOST_FOREACH(totalCost tc, cv){
        pref p;
        p.id=tc.path_id;
        p.weight=n;
        n--;
        favorites.push_back(p);
      } 
    }*/


    /*
      * order favorites path according to the total traveling time with doublecost function
      * */      
    /*void getFavorites_(pathList paths){
      
      costVector cv=ProcessTotalCost_with_doubleCost(paths);
      cv.sort(compare_cost);
      int n=cv.size()+10;
      
      BOOST_FOREACH(totalCost tc, cv){

        //cout << tc.cost <<endl;
        pref p;
        p.id=tc.path_id;
        p.weight=n;
        n--;
        favorites.push_back(p);
      } 
    }*/
  };
}