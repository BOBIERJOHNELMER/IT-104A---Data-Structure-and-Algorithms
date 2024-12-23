#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

sf::Text createText(const std::string& content, const sf::Font& font, float x, float y, unsigned int size = 24) {
    sf::Text text;
    text.setFont(font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
    return text;
}

void displayStartupWindow() {
    sf::RenderWindow startupWindow(sf::VideoMode(1024, 720), "Welcome to Heap Management!");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }
    sf::Text title = createText("Heap Management Game", font, 10, 10, 34);
    sf::Text instructions = createText(
        "\n\n\n\nBackstory:\n\nIn the far future, humans have built colonies on planets across the galaxy.\n\n"
        "These colonies depend on shipments of important supplies like food, medicine,\n\n"
        "and tools. You are the cargo manager on the SS Nexus. Your job is to prioritize\n\n"
        "and deliver shipments efficiently to keep the colonies thriving.\n\n\n\n"
        "Mission:\n\n"
        "1. Insert Shipments: Add new shipments without disrupting priority.\n\n"
        "2. Dispatch Shipments: Deliver the most urgent shipments.\n\n"
        "3. Maintain Order: Use the Heapify process to keep the system organized.\n\n"
        "4. Monitor Changes: Track how actions impact priorities.\n\n"
        "\n\n\n\n\n\n\n\nPress any key to start...",
        font, 50, 10, 20
    );
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("startup_background.jpg")) {
        std::cerr << "Error: Could not load startup_background.jpg!" << std::endl;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    while (startupWindow.isOpen()) {
        sf::Event event;
        while (startupWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                startupWindow.close();
            if (event.type == sf::Event::KeyPressed) {
                startupWindow.close();
            }
        }
        startupWindow.clear();
        if (backgroundTexture.getSize().x > 0) {
            startupWindow.draw(backgroundSprite);
        }
        startupWindow.draw(title);
        startupWindow.draw(instructions);
        startupWindow.display();
    }
}

void instructions() {
    sf::RenderWindow startupWindow(sf::VideoMode(1024, 720), "Welcome to Heap Management!");
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }
    sf::Text title = createText("Heap Management Game", font, 10, 10, 34);
    sf::Text instructions = createText(
        "Use arrow keys to navigate the bot.\n"
        "Door: Insert shipments into the heap.\n"
        "Ship: Dispatch shipments.\n"
        "Box: View the heap contents.\n\n"
        "Go near the to perform the task\n\n\n\n"
        "Press any key to start...",
        font, 100, 200, 24
    );
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("startup_background.jpg")) {
        std::cerr << "Error: Could not load startup_background.jpg!" << std::endl;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    while (startupWindow.isOpen()) {
        sf::Event event;
        while (startupWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                startupWindow.close();
            if (event.type == sf::Event::KeyPressed) {
                startupWindow.close();
            }
        }
        startupWindow.clear();
        if (backgroundTexture.getSize().x > 0) {
            startupWindow.draw(backgroundSprite);
        }
        startupWindow.draw(title);
        startupWindow.draw(instructions);
        startupWindow.display();
    }
}

void buildHeapMin(std::vector<int>& heap) {
    int n = heap.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        int smallest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && heap[left] < heap[smallest])
            smallest = left;
        if (right < n && heap[right] < heap[smallest])
            smallest = right;
        if (smallest != i) {
            std::swap(heap[i], heap[smallest]);
            buildHeapMin(heap);
        }
    }
}

void buildHeapMax(std::vector<int>& heap) {
    int n = heap.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        if (left < n && heap[left] > heap[largest])
            largest = left;
        if (right < n && heap[right] > heap[largest])
            largest = right;
        if (largest != i) {
            std::swap(heap[i], heap[largest]);
            buildHeapMax(heap);
        }
    }
}

void insertMinHeap(std::vector<int>& heap, int value) {
    heap.push_back(value);
    int index = heap.size() - 1;
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[parent] <= heap[index])
            break;
        std::swap(heap[parent], heap[index]);
        index = parent;
    }
}

int removeMaxHeap(std::vector<int>& heap) {
    if (heap.empty()) return -1;
    int maxValue = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    buildHeapMax(heap);
    return maxValue;
}

void insertShipment(std::vector<int>& heap, std::vector<int>& availableShipments, const sf::Font& font) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Insert Shipment");
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("insert_background.jpg")) {
        std::cerr << "Error: Could not load insert_background.jpg!" << std::endl;
        return;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::String userInput;
    sf::Text instructions = createText("Enter shipment value to insert:", font, 50, 50);
    sf::Text availableText = createText("", font, 50, 100);
    sf::Text inputText = createText("", font, 50, 200);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    window.close();
                }
            }

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !userInput.isEmpty())
                    userInput.erase(userInput.getSize() - 1);
                else if (event.text.unicode == '\r') {
                    std::string input = userInput.toAnsiString();
                    try {
                        int value = std::stoi(input);
                        auto it = std::find(availableShipments.begin(), availableShipments.end(), value);
                        if (it != availableShipments.end()) {
                            insertMinHeap(heap, value);
                            availableShipments.erase(it);
                        }
                    }
                    catch (...) {}
                    userInput.clear();
                }
                else if (event.text.unicode < 128) {
                    userInput += event.text.unicode;
                }
            }
        }
        std::stringstream ss;
        ss << "Available Shipments: ";
        for (int shipment : availableShipments)
            ss << shipment << " ";
        availableText.setString(ss.str());
        inputText.setString("Input: " + userInput);
        window.clear();
        window.draw(backgroundSprite);
        window.draw(instructions);
        window.draw(availableText);
        window.draw(inputText);
        window.display();
    }
}

void dispatchShipment(std::vector<int>& heap, const sf::Font& font) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Dispatch Shipment");
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("dispatch_background.gif")) {
        std::cerr << "Error: Could not load dispatch_background.gif!" << std::endl;
        return;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::Text text = createText("", font, 50, 50);
    if (!heap.empty()) {
        buildHeapMax(heap);
        int dispatched = removeMaxHeap(heap);
        text.setString("Dispatched shipment: " + std::to_string(dispatched));
    }
    else {
        text.setString("No shipments to dispatch.");
    }
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    window.close();
                 
                }
            }

        }
        window.clear();
        window.draw(backgroundSprite);
        window.draw(text);
        window.display();
    }
}


void viewHeap(const std::vector<int>& heap, const sf::Font& font) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "View Heap");
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("view_background.png")) {
        std::cerr << "Error: Could not load view_background.png!" << std::endl;
        return;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    std::vector<int> sortedHeap = heap;
    std::sort(sortedHeap.begin(), sortedHeap.end());

    std::stringstream ss;
    for (int item : sortedHeap)
        ss << item << " ";
    sf::Text text = createText("Heap contents (Min-Heap): " + ss.str(), font, 50, 50);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    window.close();
                }
            }
        }
        window.clear();
        window.draw(backgroundSprite);
        window.draw(text);
        window.display();
    }
}

int main() {
    displayStartupWindow();
    instructions();

    sf::RenderWindow window(sf::VideoMode(800, 600), "Heap Management");
    window.setFramerateLimit(60);

    sf::Music music;
    if (!music.openFromFile("background_music.mp3")) {
        std::cerr << "Error: Could not load background music!" << std::endl;
        return -1;
    }
    music.setLoop(true);
    music.play();

    sf::Texture botTexture;
    if (!botTexture.loadFromFile("bot.png")) {
        std::cerr << "Error: Could not load bot.png!" << std::endl;
        return -1;
    }

    sf::Sprite botSprite(botTexture);
    const int frameWidth = 64, frameHeight = 68;
    sf::IntRect currentFrame(0, 0, frameWidth, frameHeight);
    botSprite.setTextureRect(currentFrame);

    sf::Vector2f position(375.f, 275.f);
    botSprite.setPosition(position);

    sf::Texture mainBackgroundTexture;
    if (!mainBackgroundTexture.loadFromFile("main_background.png")) {
        return -1;
    }
    sf::Sprite mainBackground(mainBackgroundTexture);

    float speed = 4.f;
    int frameIndex = 0, currentRow = 0;
    sf::Clock animationClock;
    float animationSpeed = 0.2f;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    std::vector<int> minHeap, availableShipments = { 12, 35, 9, 18, 23 };

    sf::Texture insertTexture, dispatchTexture, viewTexture;
    if (!insertTexture.loadFromFile("red.png") || !dispatchTexture.loadFromFile("blue.png") || !viewTexture.loadFromFile("yellow.png")) {
        std::cerr << "Error: Could not load square textures!" << std::endl;
        return -1;
    }

    sf::Sprite insertSquare(insertTexture), dispatchSquare(dispatchTexture), viewSquare(viewTexture);

    insertSquare.setPosition(20.f, 20.f);
    dispatchSquare.setPosition(window.getSize().x - 70.f, 20.f);
    viewSquare.setPosition(20.f, window.getSize().y - 70.f);

    sf::Clock cooldownTimer;
    bool canTrigger = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    window.close();
                }
            }
        }

        bool isMoving = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            position.y -= speed;
            currentRow = 3;
            isMoving = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            position.y += speed;
            currentRow = 0;
            isMoving = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            position.x -= speed;
            currentRow = 1;
            isMoving = true;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            position.x += speed;
            currentRow = 2;
            isMoving = true;
        }

        if (isMoving) {
            if (animationClock.getElapsedTime().asSeconds() >= animationSpeed) {
                frameIndex = (frameIndex + 1) % 4;
                currentFrame.left = frameIndex * frameWidth;
                currentFrame.top = currentRow * frameHeight;
                botSprite.setTextureRect(currentFrame);
                animationClock.restart();
            }
        }
        else {
            frameIndex = 0;
            currentFrame.left = frameIndex * frameWidth;
            currentFrame.top = currentRow * frameHeight;
            botSprite.setTextureRect(currentFrame);
        }

        botSprite.setPosition(position);

        if (botSprite.getGlobalBounds().intersects(insertSquare.getGlobalBounds()) && canTrigger) {
            insertShipment(minHeap, availableShipments, font);
            cooldownTimer.restart();
            canTrigger = false;
        }
        else if (botSprite.getGlobalBounds().intersects(dispatchSquare.getGlobalBounds()) && canTrigger) {
            dispatchShipment(minHeap, font);
            cooldownTimer.restart();
            canTrigger = false;
        }
        else if (botSprite.getGlobalBounds().intersects(viewSquare.getGlobalBounds()) && canTrigger) {
            viewHeap(minHeap, font);
            cooldownTimer.restart();
            canTrigger = false;
        }

        if (cooldownTimer.getElapsedTime().asSeconds() >= 4.f) {
            canTrigger = true;
        }

        window.clear();
        window.draw(mainBackground);
        window.draw(botSprite);
        window.draw(insertSquare);
        window.draw(dispatchSquare);
        window.draw(viewSquare);
        window.display();
    }

    return 0;
}










