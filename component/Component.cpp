
#include "Component.h"


Components Component::ourCurrentComponents;


void
Component::RegisterComponent(
        Component* component
        )
{
    ourCurrentComponents.push_back(component);
}

const Components&
Component::GetRegisteredComponents()
{
    return ourCurrentComponents;
}

void
Component::ClearRegisteredComponents()
{
    ourCurrentComponents.clear();
}
