
#include "XMLElement.h"


std::ostream&
operator<<(
        std::ostream&       outputStream,
        const XMLElement&   element
        )
{
    element.write(outputStream);

    return outputStream;
}


XMLElements::XMLElements() :
    std::vector<XMLElement*>()
{
}

XMLElements::~XMLElements()
{
    for(
            std::vector<XMLElement*>::const_iterator elementIter = begin();
            elementIter != end();
            ++elementIter
       )
    {
        delete (*elementIter);
    }
}

void
XMLElements::add(
        XMLElement* element
        )
{
    push_back(element);
}

void
XMLElements::write(
        std::ostream& outputStream
        ) const
{
    for(
            std::vector<XMLElement*>::const_iterator elementIter = begin();
            elementIter != end();
            ++elementIter
       )
    {
        XMLElement* element = *elementIter;
        
        outputStream << (*element);
    }
}

std::ostream&
operator<<(
        std::ostream&       outputStream,
        const XMLElements&  elements
        )
{
    elements.write(outputStream);

    return outputStream;
}

