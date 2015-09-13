#ifndef TWODIMCONTROLLER_H
#define TWODIMCONTROLLER_H

/**
 * Interface definition for a controller with two dimensions
 */
class TwoDimController
{
    public:

        /**
         * Destructor
         */
        virtual ~TwoDimController(){}

        /**
         * Provides this controller's position on the X axis
         *
         * \return A position value between -1 and 1.
         */
        virtual float getXPosition() = 0;

        /**
         * Provides this controller's position on the Y axis
         *
         * \return A position value between -1 and 1.
         */
        virtual float getYPosition() = 0;
};

#endif /* ifndef TWODIMCONTROLLER_H */
