#include <boost/graph/edge_list.hpp>
#include <fstream>

#include <vector>

#include <algorithm>
#include <iostream>
#include <cassert>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <tuple>
#include <map>
#include <queue>

typedef std::pair<int,int> Edge;

using namespace std;
using namespace boost;

//map: evento -> lista di coppie: (srcId, dstId)
typedef vector<Edge> Lista_archi;
typedef std::map<int, Lista_archi> My_Map;

typedef property<edge_name_t, int> event;
typedef adjacency_list<mapS, vecS, undirectedS,no_property,event> Graph;

#define OK 0
#define NOCROSS 1
#define EXIT_NOCROSS 2
#define ENTER_NOCROSS 3

int num_stati, num_transazioni, stato_iniziale, num_eventi;

typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;

My_Map* ts_map = new My_Map();

// declare a graph object
Graph g(0);
Vertex* vertex_array;

typedef set<int> Region;
typedef set<int>* ER;

vector<ER>* ER_set = new  vector<ER>;
vector<Region>* pre_regions = new vector<Region>;

vector<Region> *queue_temp_regions= new vector<Region>;


map< int , vector< set<int>* > * > *map_states_to_add= new map< int , vector< set<int>* > * > ();
vector< set<int>* > *vec_states_to_add;
set<int>* states_to_add_enter;
set<int>* states_to_add_nocross;

void parser(){
    // Open the file:
    std::ifstream fin("../input.txt");

    assert(fin);

// Read defining parameters:
    fin >> num_stati ;
    fin >> num_transazioni;
    fin >> stato_iniziale;

    /* cout << num_stati << endl;
     cout << num_transazioni << endl;
     cout << stato_iniziale << endl;*/

    vertex_array = new Vertex[num_stati];

    for(int i = 0; i < num_stati; i++){
        vertex_array[i]=boost::add_vertex(g);
    }

    int src, dst, ev;

    //aggiungo gli archi al grafo
    for (int i = 0; i < num_transazioni; ++i) {
        //add_edge(get<0>(transaction_array[i]), get<1>(transaction_array[i]),event(10), g);
        //add_edge(vertex_array[], vertex_array[],event(100), g);
        fin >> src;
        fin >> dst;
        fin >> ev;
        add_edge(vertex_array[src], vertex_array[dst], event(ev), g);
        //non c'è l'entry relativa all'evento ev
        if (ts_map->find(ev) == ts_map->end()){
            (*ts_map)[ev] = Lista_archi();
            //mappa.insert(Mappa::value_type(ev, Lista_archi()));
        }
        (*ts_map).at(ev).push_back(std::make_pair(src, dst));

    }

    num_eventi = (*ts_map).size();

    property_map<Graph, edge_name_t>::type
            eventMap = get(edge_name_t(), g);

    boost::graph_traits< Graph >::edge_iterator e_it, e_end;
    for(std::tie(e_it, e_end) = boost::edges(g); e_it != e_end; ++e_it)
    {
        std::cout << boost::source(*e_it, g) << " "
                  << boost::target(*e_it, g) << " "
                  << eventMap[*e_it] << std::endl;
    }


    fin.close();
}

ER createER(int event){
    ER er = new set<int>;
    for(auto edge: (*ts_map)[event]){
        (*er).insert(edge.first);
        cout<< "CREATE ER: Insert state: " << edge.first <<endl;
    }

    for(auto i: *er){
        cout<< "S: " << i <<endl;
    }

    return er;
}

int event_type(Lista_archi* list, Region *region, int event){
 // quale ramo devo prendere tra ok, nocross oppure 2 rami? (per un evento)
    vector<int> *trans= new vector<int>(4,0);

    vec_states_to_add= new vector< set<int>* >;
    states_to_add_enter=new set<int>;
    states_to_add_nocross=new set<int>;

//num in-out-exit-enter
    const int in=0;
    const int out=1;
    const int exit=2;
    const int enter=3;

    (*map_states_to_add)[event]= vec_states_to_add;



    for(auto t: *list){
        if( region->find(t.first) != region->end()){ //il primo stato appartiene alla regione
            if(region->find(t.second) != region->end()) { //anche il secondo stato appartiene alla regione
                (*trans)[in]++;
                cout<< t.first << "->" <<t.second << " IN " <<endl;
            }
            else {
                (*trans)[exit]++;
                cout<< t.first << "->" <<t.second << " EXIT" <<endl;
            }
        }
        else {//il primo non ci appartiene
            if(region->find(t.second) != region->end()) { //il secondo stato appartiene alla regione
                (*trans)[enter]++;
                cout<< t.first << "->" <<t.second << " ENTER" << endl;
                //per il no cross devo aggiungere la sorgente di tutti gli archi entranti nella regione(enter diventa in)
                //mappa di int(evento) e vettore di puntatori a insiemi di stati da aggiungere
                (*states_to_add_nocross).insert(states_to_add_nocross-> begin(), t.first);
                cout<< "inserisco " << t.first << " per nocross " << endl;
            }
            else {
                (*trans)[out]++;
                cout<< t.first << "->" <<t.second << " OUT" << endl;
                //per enter devo aggiungere la destinazione degli archi che erano out dalla regione
                (*states_to_add_enter).insert(states_to_add_enter-> begin(), t.second);
                cout<< "inserisco " << t.second << " per enter " <<endl;
            }
        }
    }

    int it=0;
    cout<< "IN/OUT/EXIT/ENTER" << endl;
    for(auto i: *trans){
        cout<<"num trans "<< it <<": " << i <<endl ;
        it++;
    }

    //
    //TODO: dealloca prima della return ????
    //gli Enter+in devono diventare per forza in(nocross)
    if( ( (*trans)[in]>0 && (*trans)[enter]>0) || ((*trans)[in]>0 && (*trans)[exit]>0) || ( (*trans)[enter]>0 && (*trans)[exit]>0 ) ) {
        cout<<"return no cross"<<endl;
        return NOCROSS;
    }
    else if( (*trans)[exit]>0 ){ //(exit-out)
        cout<<"return exit_no cross"<<endl;
        return EXIT_NOCROSS;
    }
    else if( (*trans)[enter]>0  ){ //(enter-out)
        cout<<"return enter_no cross"<<endl;

        //aggiungo gli stati da aggiungere per entry e no cross (ma li aggiunge alla coda la expand per controllare che sia il ramo giusto da prendere)
        vec_states_to_add->push_back(states_to_add_enter);
        vec_states_to_add->push_back(states_to_add_nocross);

        return ENTER_NOCROSS;
    }
    else {
        cout<<"return ok"<<endl;
        return OK;
    }


}

void expand(Region *region, int event){
    int* event_types = new int[num_eventi];
    int last_event_2braches=-1;
    vector<Region*> * expanded_regions = new vector<Region*>();


    for(auto i: (*region))
        cout<< "Regione: "<< i << endl;


    for(auto e: *ts_map){
        cout<< "EVENTO: " <<e.first<<endl;
        //controllo tutti, non è un ER
        if(event == -1) {
            event_types[e.first] = event_type(&e.second, region, e.first);
            cout << "Non è ER" << endl;
        }
        //è un ER non controllo l'evento relativo all'ER
        else if(e.first != event && event != -1) {
            cout << " è un ER di " << event <<endl;
            event_types[event] = OK;
            event_types[e.first] = event_type(&e.second,region, e.first);
        }
    }
    int branch = OK;
    int pos;
    int type;
    for(int i = 0; i < num_eventi; i ++){
        type=event_types[i];
        if(type == NOCROSS){
            cout<<"Break per no_cross " <<endl;
            branch = NOCROSS;

            break;
        }
        if(type == EXIT_NOCROSS){
            if(branch == OK) {
                branch = EXIT_NOCROSS;
                last_event_2braches=i;
            }
            cout<<"2 rami exit" <<endl;
        }
        else if(type == ENTER_NOCROSS){
            if(branch == OK) {
                branch = ENTER_NOCROSS;
                last_event_2braches=i;
            }
            cout<<"2 rami enter" <<endl;
            cout << "branch: " << branch << endl;
        }

    }

    if(branch == OK){
        cout<<"OK" <<endl;
        (*pre_regions).push_back(*region); //aggiunta pre-regione giusta
    }
    else if (branch == NOCROSS){
        cout<<"NO CROSS" <<endl;
        //capire gli stati da aggiungere
        //l'operazione sta nella copia della regione puntata, l'espansione di tale regione e il ritorno di una nuova regione più grande
        //mettere l'unico ramo (regione successiva)
    }
    else{
        //aggiungere alla coda i 2 prossimi rami (2 regioni successive)
        if(branch==ENTER_NOCROSS){

            //per il no cross devo aggiungere la sorgente di tutti gli archi entranti nella regione(enter diventa in)
            //per enter devo aggiungere la destinazione degli archi che erano out dalla regione

            cout<< "RAMO ENTER_NOCROSS " << endl;
            (*region).insert(region->begin(), 1);
            cout << "dim region " << (*region).size() << endl;
            /*set<int>::iterator it;

            for(it = region->begin(); it != region->end();it++){
                (*expanded_region)->insert(*it);
                cout<< "inserisco nella extended Reg: " << *it << endl;

            }*/

            for(auto state: *region){

                (expanded_regions)[0].insert((expanded_regions)[0].begin(), state);
                cout<< "inserisco nella extended Reg: " << state << endl;
            }

            for(auto i: *region){
                cout << "Stati region " << i <<endl ;
            }

            cout << "last evvent 2 branches index: " << last_event_2braches << endl;
            cout << "map states to add size: " << (*map_states_to_add).size() << endl;

            for(auto key: (*map_states_to_add)){
                cout << "chiave: " << key.first << " ";
                cout << "valore: " << key.second << endl;
            }

            auto vec=(*map_states_to_add).at(last_event_2braches);


            cout << "dim primo set vettore: " << (*vec).front()->size() << endl;

            //for(auto set : *vec ){
            for(auto state : (vec)[0] )
                (expanded_regions)[0].insert((expanded_regions)[0].begin(), state);
            for(auto state : (vec)[1] )
                (expanded_regions)[1].insert((expanded_regions)[1].begin(), state);
            //}


            for(auto i: expanded_regions[0]){
                cout << "Stato della regione espansa " << i <<endl ;
            }



        }
    }

}


int main()
{
    bool first;
    parser();
    int pos=0;

    for(auto e : *ts_map){
        ER er_temp = createER(e.first);
        (*ER_set).push_back(er_temp);

        //espando la prima volta - la regione coincide con ER
        expand(er_temp, e.first);

        while(pos!=queue_temp_regions->size()){
                expand(&((*queue_temp_regions)[pos]), -1);
                pos++;
                //tolgo l'elemento espanso dalla coda
               // queue_temp_regions->pop_front();
        }

    }

    delete ER_set;
    delete map_states_to_add;
    delete queue_temp_regions;
    delete pre_regions;
}
