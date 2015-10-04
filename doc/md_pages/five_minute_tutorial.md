5 minute Tutorial  {#five_minute_tutorial_page}
=================
This shortcut tutorial will show you all basic information to use RTTR.

~~~~{.cpp}
// widgeth.h
#include <rttr/registration>

class widget
{
public:
    widget();
    widget(std::string name);
    virtual ~widget();

    void set_name(std::string name);
    std::string get_name() const;

    bool is_static() const;

    void set_size(int width, int height);
    void set_size(const size& new_size);
    size get_size() const;

    std::vector<widget> get_children() const;
  
    virtual void render(widget* parent, E_Render_Flag ts = E_Render_Flag::DRAW_CHILDREN);

  
private:
    size                m_size;
    std::string         m_name;
    bool                m_is_static;
    std::vector<widget> m_children;

    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND
};
~~~~


~~~~{.cpp}
// button.h
#include <rttr/registration>

class button : public widget
{
public:
    button(std::string name);
    static std::shared_ptr<button> create_button(std::string name = "default button");

    virtual void render(widget* parent, E_Render_Flag ts = E_Render_Flag::DRAW_CHILDREN);

    RTTR_ENABLE(widget)
};
~~~~

<hr>

<div class="btn btn-default">[back](@ref tutorial_page "Tutorial")</div>
