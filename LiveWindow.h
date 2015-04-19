#ifndef LIVEWINDOW_H
#define LIVEWINDOW_H


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

#endif /* ifndef LIVEWINDOW_H */
