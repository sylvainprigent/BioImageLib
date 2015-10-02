#include <blMpp>
#include <blTxtIO>

class blMppFactory{

public:
    blMppFactory(std::string parameterFile);
    blMppFactory(blProcessParameters* parameters);
    ~blMppFactory();

public:
    blMppGeneratorShape* generator(std::string name);
    blMppDataTerm* dataTerm(std::string name);
    blMppInteraction* interaction(std::string name);
    blMppAlgorithm* algorithm(blMppGeneratorShape* generator, blMppDataTerm* dataTerm, blMppInteraction*interaction, std::string name);

private:
    blProcessParameters* m_parameters;
    blProcessObserverCommandeLine *m_observer;
};
