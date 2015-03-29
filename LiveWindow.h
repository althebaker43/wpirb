#ifndef LIVEWINDOW_H
#define LIVEWINDOW_H


class LiveWindow
{
    public:

        static LiveWindow* GetInstance();

        void Run();

    private:

        static LiveWindow* myInstance;

        LiveWindow();
        LiveWindow(const LiveWindow&);
        ~LiveWindow();
};

#endif /* ifndef LIVEWINDOW_H */
