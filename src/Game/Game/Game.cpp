#include "core.hpp"
#include "Context.hpp"
#include "GraphicEngine.hpp"
#include "input-dispatcher.hpp"
#include "input.hpp"
#include "Game.hpp"
#include "GameEvents.hpp"
#include "Logging.hpp"
#include "Settings.hpp"
#include "Texture.hpp"
#include "ui-renderer.hpp"
#include "ui-text.hpp"
#include "ui.hpp"
#include "ui-widgets.hpp"

struct GameViewState
{
    ~GameViewState() = default;
    virtual bool execute(Imgui& ui) = 0; // false means that state is finished
};

struct GameSettings : public GameViewState
{
private:
    Settings& m_settings;
    Settings m_settingsBackup;
    enum Views {Misc, Video, Audio, Controls};
    int m_currentPanel = Misc;
    bool m_firstRun {true};

    void drawMisc(Imgui& ui, Panel& parentPanel){
        Panel panel(parentPanel);
        panel.color(0)();
        panel.layout().padding({20,20,20,20}).spacing(25).toDown(LEFT);
        panel.item().w(250).h(35)().text("Nothing to se here..");
    }

    DropDownList<glm::vec2> m_windowSizes;
    void drawVideo(Imgui& ui, Panel& parentPanel){
        Panel panel(parentPanel);
        panel.color(0)();
        panel.layout().padding({20,20,20,20}).spacing(25).toDown(LEFT);

        // ? panel.checkbox().w(160).h(35)(m_settings.fullscreen);
        checkbox(m_settingsBackup.video.fullscreen, "Fullscreen", panel, {250, 35});
        m_windowSizes.execute(panel, {250, 35});

    }
    void drawAudio(Imgui& ui, Panel& parentPanel){
        Panel panel(parentPanel);
        panel.color(0)();
        panel.layout().padding({20,20,20,20}).spacing(25).toRight();

        Panel column_a(panel);
        column_a.width(250).color(0)();
        column_a.layout().spacing(20).toDown();

        Panel column_b(panel);
        column_b.width(250).color(0)();
        column_b.layout().spacing(20).toDown();
            int h = 35;
            column_a.item().h(h).color(0)().text("Master Volume");
            column_b.slider().h(h)(m_settingsBackup.audio.masterVolume, 0.f, 100.f);

            column_a.item().h(h).color(0)().text("Music Volume");
            column_b.slider().h(h)(m_settingsBackup.audio.musicVolume, 0.f, 100.f);

            column_a.item().h(h).color(0)().text("Effects Volume");
            column_b.slider().h(h)(m_settingsBackup.audio.effectsVolume, 0.f, 100.f);

            column_a.item().h(h).color(0)().text("Voice Volume");
            column_b.slider().h(h)(m_settingsBackup.audio.voiceVolume, 0.f, 100.f);

    }
    void drawControls(Imgui& ui, Panel& parentPanel){

    }

public:
    GameSettings(Settings& settings) : m_settings(settings), m_windowSizes({{{"1600x900"}, {1600,900}}, {{"1920x1080"}, {1920,1080}}, {{"1920x1200"},{1920,1200}}}, {{"1600x900"}, {1600,900}}){}
    bool execute(Imgui& ui) override {
        if(m_firstRun){
            m_settingsBackup = m_settings;
            m_firstRun = false;
        }

        Panel panel(ui);
        panel.width(0.8f).height(0.8f)
            .x(0.1f).y(0.1f)
            .blured(0x10101080)();
        panel.layout().padding({20,5,20,20}).spacing(25).toDown(notEven({40, 1.f, 40}));
        if(panel.onKey("on-esc")){
            return false;
            m_firstRun = true;
        }

            Panel header(panel);
            header.color(0)();
            header.layout().toRight(even(4));
            header.quickStyler([this](Item& item){
                item.formatting(Text::Centered).color(0);
                if(item.id()==m_currentPanel) item.font("ui_20_bold");
                item().action([this, &item]{m_currentPanel = item.id();});
            });
            header.button().text("Misc.");
            header.button().text("Video");
            header.button().text("Audio");
            header.button().text("Controls");

        switch(m_currentPanel){
            case Misc: drawMisc(ui, panel); break;
            case Video: drawVideo(ui, panel); break;
            case Audio: drawAudio(ui, panel); break;
            case Controls: drawControls(ui, panel); break;
            default:
                log("unknown panel", m_currentPanel);
        }

        Panel savebar(panel);
        savebar.color(0)();
        savebar.layout().toLeft(DOWN).spacing(10);
        savebar.item().w(150).color(0)().text("Save").action([this]{
            m_settings = m_settingsBackup;
            m_settings.save();
            m_firstRun = true;
        });


        return true;
    }
};

struct GameUI
{
private:
    std::function<void(void)> m_callAfterTransition;
    enum State {
        FadeIn, Main, FadeOut, External
    };
    State m_state;
    const float fadeTime = 0.1f * 1000;

    void fadeIn(){
        m_state = FadeIn;
        m_timer = fadeTime;
    }
    void fadeOut(std::function<void(void)> f){
        m_state = FadeOut;
        m_timer = fadeTime;
        m_callAfterTransition = f;
    }

    void render(Imgui& ui, float startPosition){
        // ui.panel().image("Logo").position(0.f, 1.f)();

        Panel panel(ui);
        panel.width(350).height(1.f)
            .x(startPosition).y(0)
            .blured(0x10101010)();
        panel.layout().spacing(23).padding({10,55,10,50}).toUp();
        panel.quickStyler([](Item& item){
            item.w(0.9f).h(44).formatting(Text::Centered);
        });
        // panel.emptySpace(0.6f);
        panel.button()().text("Exit").action([this]{
            fadeOut([]{event<ExitGame>();});
        });
        panel.button()().text("Credits").action([this]{
            fadeOut([this]{log("Credits? Me! Lazy Falcon!");fadeIn();});
        });
        panel.button()().text("Settings").action([this]{
            fadeOut([this]{fadeOut([this]{ m_currentState = m_settings; });});
        });
        panel.button()().text("Continue");
        if(m_playSelected){
            panel.button().w(0.8f).h(40)().text("Demo With Drones").action([]{
                log("not implemented yet");
            });
            panel.button().w(0.8f).h(40)().text("Void Rendering").action([]{
                log("not implemented yet");
            });
            panel.button().w(0.8f).h(40)().text("Graphic Testing").action([]{
            });
        }
        panel.button()().text("New Game").action([this]{m_playSelected = !m_playSelected;});
        // panel.button().y(0.6f)().text("New Game").action([this]{m_playSelected = !m_playSelected;});
        if(panel.onKey("on-esc")){
            fadeOut([]{event<ExitGame>();});
        }

    }

    std::shared_ptr<GameViewState> m_currentState;
    std::shared_ptr<GameViewState> m_settings;

    float toSlide {45.f};
    float m_timer;
    float m_uiPos;
    bool m_playSelected {false};
public:
    GameUI(Settings& settings) : m_settings(std::make_shared<GameSettings>(settings)){
        fadeIn();
    }
    void run(Imgui& ui, float dt){
        switch(m_state){
            case FadeIn :
                m_timer -= dt;
                if(m_timer < 0.f) m_state = Main;

                m_uiPos = -1 - 450 * glm::smoothstep(0.f, 1.f, 1-m_timer / fadeTime);

                break;
            case Main :
                break;
            case FadeOut :
                m_timer -= dt;
                if(m_timer < 0.f){
                    m_state = External;
                    m_callAfterTransition();
                    return;
                }

                m_uiPos = -450 + 449 * glm::smoothstep(0.f, 1.f, 1-m_timer / fadeTime);

                break;
            case External:
                if(m_currentState){
                    if(m_currentState->execute(ui)) return;
                    else {
                        m_currentState.reset();
                        fadeIn();
                    }
                }
                return;
        }
        render(ui, m_uiPos);
    }

};

Game::Game(Imgui& ui, InputDispatcher& inputDispatcher, Settings& settings): m_ui(ui), m_input(inputDispatcher.createNew("Game")){
    m_view = std::make_unique<GameUI>(settings);
    m_input->activate();
}
Game::~Game(){
    m_input->deactivate();
}
void Game::update(float dt){
    // m_ui.update();
    m_view->run(m_ui, dt);

}
void Game::updateWithHighPrecision(float dt){}
void Game::renderProcedure(GraphicEngine& renderer){
    renderer.context->beginFrame();
    renderer.context->endFrame();
}
