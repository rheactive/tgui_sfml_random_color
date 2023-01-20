#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <sstream>
#include <iomanip>
#include <iostream>
#include <random>

#include <TGUI/TGUI.hpp>

const float WIDTH = 1600;
const float HEIGHT = 900;
const float margin_x = 20;
const float margin_y = 20;
const float plot_width = 900;
const float plot_height = HEIGHT - 2 * margin_y;
const float plot_x = margin_x;
const float plot_y = margin_y;
const float GUI_width = WIDTH - plot_width - 3 * margin_x;
const float GUI_height = plot_height;
const float GUI_x = 2 * margin_x + plot_width;
const float GUI_y = margin_y;
const int fsz = 35;

void stop(bool& running) {
    running = false;
};

void change_color(
    sf::Image& image, 
    sf::Texture& texture, 
    sf::Sprite& sprite,
    int red, int green, int blue) {
    image.create(plot_width, plot_height, sf::Color(red, green, blue));
    texture.loadFromImage(image);
    sprite.setTexture(texture);
};


int main()
{
    int frame_count = 0;
    int current_fps = 60;
    float current_time1;
    float current_time2;
    sf::Color bg_color = sf::Color::White;
    bool color_should_change = false;
    float scale_x = 1;
    float scale_y = 1;
    int fsz1 = fsz;
    int Red = 255;
    int Green = 255;
    int Blue = 255;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> color(0,255);

    // create the window
    sf::RenderWindow window;
    window.create(sf::VideoMode(WIDTH, HEIGHT), "GUI example", sf::Style::Default);
    window.setFramerateLimit(60);
    // activate the window
    window.setActive(true);

    // create GUI
    tgui::GuiSFML gui{window};

    tgui::Label::Ptr label1 = tgui::Label::create("FPS: ");
    gui.add(label1);
    label1 -> setTextSize(fsz1);
    label1 -> setPosition(GUI_x, margin_y);
    label1 -> getRenderer() -> setTextColor(sf::Color::White);

    tgui::Button::Ptr button1 = tgui::Button::create("Exit");
    gui.add(button1);
    button1 -> setPosition(GUI_x, 5 * margin_y);
    button1 -> setTextSize(fsz1);

    tgui::Label::Ptr label2 = tgui::Label::create("Color: ");
    gui.add(label2);
    label2 -> setTextSize(fsz1);
    label2 -> setPosition(GUI_x, 9 * margin_y);
    label2 -> getRenderer() -> setTextColor(sf::Color::White);

    tgui::EditBox::Ptr edit_box1 = tgui::EditBox::create();
    gui.add(edit_box1);
    edit_box1 -> setTextSize(fsz1);
    edit_box1 -> setPosition(GUI_x, 13 * margin_y);
    edit_box1 -> setSize(7 * fsz1, 1.2 * fsz1);
    edit_box1 -> setAlignment(tgui::EditBox::Alignment::Center);
    edit_box1 -> setReadOnly(false);

    tgui::Button::Ptr button2 = tgui::Button::create("Change color");
    gui.add(button2);
    button2 -> setPosition(GUI_x, 17 * margin_y);
    button2 -> setTextSize(fsz1);

    // load resources, initialize the OpenGL states, ...
 
    sf::Clock clock1;
    sf::Clock clock2;

    sf::Image image;
    image.create(plot_width, plot_height, bg_color);

    sf::Texture texture;
    texture.loadFromImage(image);

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(plot_x, plot_y);

    // run the main loop
    bool running = true;
    while (running)
    {
        window.clear(sf::Color::Transparent);
        if (color_should_change) {
            Red = color(rng);
            Green = color(rng);
            Blue = color(rng);
            change_color(image, texture, sprite, Red, Green, Blue);
        };
        color_should_change = false;

        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            gui.handleEvent(event);
            button1 -> onClick([&]{ stop(running); });
            button2 -> onClick([&]{ color_should_change = true; });
            //button2 -> onClick(&change_color, &image, &texture, &sprite, color(rng), color(rng), color(rng));


            if (event.type == sf::Event::Closed)
            {
                // end the program
                stop(running);
            }
            else if (event.type == sf::Event::Resized)
            {
                scale_x = float(window.getSize().x) / WIDTH;
                scale_y = float(window.getSize().y) / HEIGHT;
                fsz1 = int(fsz * scale_x);
                button1 -> setTextSize(fsz1);
                button2 -> setTextSize(fsz1);
                label1 -> setTextSize(fsz1);
                label2 -> setTextSize(fsz1);
                edit_box1 -> setTextSize(fsz1);
                edit_box1 -> setSize(7 * fsz1, 1.2 * fsz1);

                label1 -> setPosition(GUI_x * scale_x, margin_y * scale_y);
                button1 -> setPosition(GUI_x * scale_x, 5 * margin_y * scale_y);
                label2 -> setPosition(GUI_x * scale_x, 9 * margin_y * scale_y);
                edit_box1 -> setPosition(GUI_x * scale_x, 13 * margin_y * scale_y);
                button2 -> setPosition(GUI_x * scale_x, 17 * margin_y * scale_y);
            }

        }

        current_time2 = clock2.getElapsedTime().asSeconds();

        // draw...
        window.draw(sprite);


        // Show FPS
        current_time1 = clock1.getElapsedTime().asSeconds();

        frame_count += 1;
        if (current_time1 >= 1.0)
        {
            current_fps = frame_count;
            frame_count = 0;
            clock1.restart();
        }

        std::ostringstream display_fps;
        display_fps << "FPS: " << current_fps;
        label1 -> setText(display_fps.str());

        std::ostringstream display_color;
        display_color << "R: " << Red << " G: " << Green << " B: " << Blue;
        label2 -> setText(display_color.str());

        std::ostringstream copy_color;
        copy_color << Red << "," << Green << "," << Blue;
        edit_box1 -> setText(copy_color.str());
        edit_box1 -> selectText();
        edit_box1 -> setFocused(true);

        // draw GUI
        gui.draw();
        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}