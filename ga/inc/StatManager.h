#ifndef STATMANAGER_H
#define STATMANAGER_H

#include <fstream>
#include <GA.h>


class StatManager {
private:
    std::vector<float> HD_distribution; 
    std::vector<long>  entities_origin;

    std::fstream result_file;     

    Entity* best;

public:
    StatManager(const GAConfig& gc);

    ~StatManager();
    
    const Entity* get_best() const; 


    void fetch_stat(size_t current_step, const EntityPool::population_t& population);
};

#endif
