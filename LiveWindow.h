#ifndef LIVEWINDOW_H
#define LIVEWINDOW_H

namespace frc
{

class LiveWindow
{
    public:

        static LiveWindow* GetInstance();

        static void DestroyInstance();

        void Run();

    private:

        static LiveWindow* ourInstance;

        LiveWindow();
        LiveWindow(const LiveWindow&);
        ~LiveWindow();
};

}; /* namespace frc */

#endif /* ifndef LIVEWINDOW_H */
