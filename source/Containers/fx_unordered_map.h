#include <unordered_map>

namespace FX
{

#define Find_Iter(ContainerWithIter, Target) ContainerWithIter.find(Target)
#define Exist_Iter(ContainerWithIter, Iter)  Iter != ContainerWithIter.end()
#define Exist_Key(ContainerWithIter, Target)  ContainerWithIter.find(Target) != ContainerWithIter.end()


}