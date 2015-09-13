#ifndef XMLELEMENT_H
#define XMLELEMENT_H

#include <vector>
#include <iostream>

/**
 * XML element interface
 */
class XMLElement
{
    public:

        /**
         * Destructor
         */
        virtual ~XMLElement(){}

        /**
         * Writes this element to the given output stream
         */
        virtual void write(
                std::ostream& outputStream
                ) const = 0;
};

/**
 * Streaming operator for XML element
 */
std::ostream& operator<<(
        std::ostream&       outputStream,
        const XMLElement&   element
        );

/**
 * Generic XML data element type
 *
 * This XML element is a simple data value surrounded by a named tag with no
 * attributes.
 */
template <class ValueType> class XMLDataElement : public XMLElement
{
    public:

        /**
         * Constructor with tag name and value
         */
        XMLDataElement(
                const char* name,
                ValueType   value
              );

        /**
         * Writes this tag to the given output stream
         */
        void write(
                std::ostream& outputStream
                ) const;

    private:

        /**
         * Name of this tag
         */
        std::string myName;

        /**
         * Value of this tag
         */
        ValueType myValue;
};

/**
 * XML element container type
 *
 * This container owns the elements contained within it, so they are deleted
 * when the container is deleted.
 */
class XMLElements : private std::vector<XMLElement*>
{
    public:

        /**
         * Default constructor
         */
        XMLElements();

        /**
         * Destructor
         *
         * This deletes all of the contained elements
         */
        virtual ~XMLElements();

        /**
         * Adds an element to this container
         */
        void add(
                XMLElement* element
                );

        /**
         * Writes the contained elements to the output stream
         */
        void write(
                std::ostream& outputStream
                ) const;
};

/**
 * Streaming operator for XML element container
 */
std::ostream& operator<<(
        std::ostream&       outputStream,
        const XMLElements&  elements
        );


// XMLDataElement implementation


template <class ValueType>
XMLDataElement<ValueType>::XMLDataElement(
        const char* name,
        ValueType   value
        ) :
    myName(name),
    myValue(value)
{
}

template <class ValueType>
void
XMLDataElement<ValueType>::write(
        std::ostream& outputStream
        ) const
{
    outputStream
        << '<' << myName << '>'
        << myValue
        << "</" << myName << '>';
}

template <class ValueType>
std::ostream&
operator<<(
        std::ostream&                       outputStream,
        const XMLDataElement<ValueType>&    tag
        )
{
    tag.write(outputStream);
    return outputStream;
}

#endif /* ifndef XMLELEMENT_H */
