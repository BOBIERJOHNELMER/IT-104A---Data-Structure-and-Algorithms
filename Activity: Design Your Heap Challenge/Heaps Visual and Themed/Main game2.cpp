#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

// Function prototypes
sf::Text createText(const string& content, const sf::Font& font, float x, float y, unsigned int size = 24);
sf::Sprite loadBackground(const string& filePath, sf::Texture& texture, float scaleX = 1.0f, float scaleY = 1.0f);
void displayIntroScreen(const sf::Font& font, const sf::Sprite& background);
void insertShipment(std::vector<int>& heap, std::vector<int>& availableShipments, const sf::Font& font, const sf::Sprite& background);
void dispatchShipment(std::vector<int>& heap, const sf::Font& font, const sf::Sprite& background);
void viewHeap(const std::vector<int>& heap, const sf::Font& font, const sf::Sprite& background);
void heapify(vector<int>& heap, int n, int i);
void buildHeap(vector<int>& heap);

// Function to create text for displaying in the window
sf::Text createText(const string& content, const sf::Font& font, float x, float y, unsigned int size) {
    sf::Text text;
    text.setFont(font); // Set the font for the text
    text.setString(content); // Set the text string
    text.setCharacterSize(size); // Set the font size
    text.setFillColor(sf::Color::Green); // Set the text color to green
    text.setPosition(x, y); // Set the position of the text in the window
    return text;
}

// Function to load and scale a background image
sf::Sprite loadBackground(const string& filePath, sf::Texture& texture, float scaleX, float scaleY) {
    if (!texture.loadFromFile(filePath)) {
        cout << "Error: Could not load " << filePath << endl; // Error loading background image
    }
    sf::Sprite sprite;
    sprite.setTexture(texture); // Set the texture for the sprite (background)
    sprite.setScale(scaleX, scaleY); // Scale the sprite to fit the window size
    return sprite;
}

// Function to maintain heap property by "heapifying" from a node downwards
void heapify(vector<int>& heap, int n, int i) {
    int largest = i; // Assume the largest element is the current node
    int left = 2 * i + 1; // Left child index
    int right = 2 * i + 2; // Right child index

    // If the left child is larger than the root
    if (left < n && heap[left] > heap[largest])
        largest = left;

    // If the right child is larger than the largest so far
    if (right < n && heap[right] > heap[largest])
        largest = right;

    // If the largest is not the root, swap and recursively heapify
    if (largest != i) {
        swap(heap[i], heap[largest]);
        heapify(heap, n, largest);
    }
}

// Function to build a max-heap from the array
void buildHeap(vector<int>& heap) {
    int n = heap.size();
    // Start from the last non-leaf node and heapify each node
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(heap, n, i);
    }
}

// Function to display the intro screen
void displayIntroScreen(const sf::Font& font, const sf::Sprite& background) {
    sf::RenderWindow introWindow(sf::VideoMode(785, 600), "Galactic Cargo Management - Intro");

    unsigned int textSize = 10; // Initial text size

    // Backstory and task text
    string content =
        "\n\n\n\nBackstory:\n\nIn the far future, humans have built colonies on planets across the galaxy.\n\n"
        "These colonies depend on shipments of important supplies like food, medicine,\n\n"
        "and tools. You are the cargo manager on the SS Nexus. Your job is to prioritize\n\n"
        "and deliver shipments efficiently to keep the colonies thriving.\n\n\n\n\n\n"
        "Mission:\n\n"
        "1. Insert Shipments: Add new shipments without disrupting priority.\n\n"
        "2. Dispatch Shipments: Deliver the most urgent shipments.\n\n"
        "3. Maintain Order: Use the Heapify process to keep the system organized.\n\n"
        "4. Monitor Changes: Track how actions impact priorities.\n\n"
        "Use UP and DOWN arrow keys to adjust text size.\n\n"
        "\n\n\n\n\n\n\n\nPress SPACE to begin...";

    sf::Text introText = createText(content, font, 30, 30, textSize); // Create the intro text

    while (introWindow.isOpen()) {
        sf::Event event;
        while (introWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                introWindow.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    introWindow.close(); // Close intro screen when SPACE is pressed
                }
                if (event.key.code == sf::Keyboard::Up) {
                    textSize = min(textSize + 2, 40U); // Increase text size, cap at 40
                }
                if (event.key.code == sf::Keyboard::Down) {
                    textSize = max(textSize - 2, 12U); // Decrease text size, floor at 12
                }
                introText.setCharacterSize(textSize); // Update text size dynamically
            }
        }

        introWindow.clear();
        introWindow.draw(background); // Draw the background image
        introWindow.draw(introText); // Draw the intro text
        introWindow.display();
    }
}

// Function to insert a new shipment
void insertShipment(vector<int>& heap, vector<int>& availableShipments, const sf::Font& font, const sf::Sprite& background) {
    sf::RenderWindow window(sf::VideoMode(785, 600), "Insert Shipment");
    sf::String userInput; // User input for shipment value
    sf::Text instructions = createText("\nEnter shipment value to insert: ", font, 50, 50);
    sf::Text availableText = createText("\nAvailable Shipments: ", font, 50, 100);
    sf::Text inputText = createText("", font, 50, 200);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !userInput.isEmpty())
                    userInput.erase(userInput.getSize() - 1); // Handle backspace
                else if (event.text.unicode == '\r') { // Handle Enter key
                    string input = userInput.toAnsiString();
                    try {
                        int value = stoi(input); // Convert input to integer
                        auto it = find(availableShipments.begin(), availableShipments.end(), value);
                        if (it != availableShipments.end()) { // If value exists in available shipments
                            heap.push_back(value); // Insert value into the heap
                            buildHeap(heap); // Rebuild heap to maintain the heap property
                            availableShipments.erase(it); // Remove the value from available shipments
                        }
                    }
                    catch (...) {
                        cout << "Invalid input. Please try again." << endl; // Handle invalid input
                    }
                    userInput.clear();
                }
                else if (event.text.unicode < 128) {
                    userInput += event.text.unicode; // Append typed character to userInput
                }
            }
        }

        stringstream ss;
        ss << "\n\n\n\nAvailable Shipments: ";
        for (int val : availableShipments) ss << val << " "; // Display available shipments
        availableText.setString(ss.str());
        inputText.setString("\n\n\n\nInput: " + userInput.toAnsiString()); // Display current input

        window.clear();
        window.draw(background); // Draw the background
        window.draw(instructions); // Draw instructions
        window.draw(availableText); // Draw available shipments
        window.draw(inputText); // Draw user input
        window.display();
    }
}

// Function to dispatch the highest-priority shipment
void dispatchShipment(vector<int>& heap, const sf::Font& font, const sf::Sprite& background) {
    sf::RenderWindow window(sf::VideoMode(785, 600), "Dispatch Shipment");

    string message;
    if (!heap.empty()) {
        int top = heap.front(); // Get the highest-priority shipment (root of the heap)
        heap[0] = heap.back(); // Replace root with the last element
        heap.pop_back(); // Remove the last element
        buildHeap(heap); // Rebuild the heap to restore heap property
        message = "Dispatched shipment with priority:\n                             " + to_string(top);
    }
    else {
        message = "No shipments to dispatch!"; // If the heap is empty, show a message
    }

    sf::Text messageText = createText(message, font, 50, 250);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(background); // Draw the background
        window.draw(messageText); // Display the dispatch message
        window.display();
    }
}

// Function to view the current heap
void viewHeap(const vector<int>& heap, const sf::Font& font, const sf::Sprite& background) {
    sf::RenderWindow window(sf::VideoMode(785, 600), "View Heap");

    stringstream ss;
    ss << "\n\nCurrent Heap: ";
    for (int val : heap) ss << val << " "; // Display all heap elements

    sf::Text heapText = createText(ss.str(), font, 50, 200);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(background); // Draw the background
        window.draw(heapText); // Draw the heap contents
        window.display();
    }
}

// Main function to drive the cargo management program
int main() {
    vector<int> heap; // Max-heap to store shipment priorities
    vector<int> availableShipments = { 12, 35, 9, 18, 23 }; // List of available shipments

    sf::RenderWindow window(sf::VideoMode(785, 600), "Cargo Management");
    sf::Font font;
    if (!font.loadFromFile("SPACEBOY.ttf")) { // Load font for text
        cout << "Error loading font!" << endl;
        return -1;
    }

    // Load background images for various screens
    sf::Texture introTexture, menuTexture, insertTexture, dispatchTexture, viewTexture;
    sf::Sprite introBackground = loadBackground("intro.jpg", introTexture, 0.5f, 0.6f);
    sf::Sprite menuBackground = loadBackground("startup.jpg", menuTexture, 0.5f, 0.6f); // Adjust the scale to fit your window size
    sf::Sprite insertBackground = loadBackground("insert.png", insertTexture, 1.7f, 1.5f);
    sf::Sprite dispatchBackground = loadBackground("dispatch.png", dispatchTexture, 1.6f, 1.6f);
    sf::Sprite viewBackground = loadBackground("view.png", viewTexture, 1.9f, 1.9f);

    // Display the introduction screen first
    displayIntroScreen(font, introBackground);

    sf::Text menuText = createText(
        "1. Insert Shipment\n\n"
        "2. Dispatch Shipment\n\n"
        "3. View Heap ",
        font, 350, 150);
    sf::String userInput;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !userInput.isEmpty())
                    userInput.erase(userInput.getSize() - 1); // Handle backspace
                else if (event.text.unicode == '\r') { // Handle Enter key
                    string input = userInput.toAnsiString();
                    if (input == "1") {
                        insertShipment(heap, availableShipments, font, insertBackground); // Call insertShipment function
                    }
                    else if (input == "2") {
                        dispatchShipment(heap, font, dispatchBackground); // Call dispatchShipment function
                    }
                    else if (input == "3") {
                        viewHeap(heap, font, viewBackground); // Call viewHeap function
                    }
                    userInput.clear();
                }
                else if (event.text.unicode < 128) {
                    userInput += event.text.unicode; // Append typed character to userInput
                }
            }
        }

        sf::Text inputText = createText("Enter number: " + userInput.toAnsiString(), font, 350, 500);

        window.clear();
        window.draw(menuBackground); // Use the menu background here
        window.draw(menuText); // Display menu options
        window.draw(inputText); // Display user input
        window.display();
    }

    return 0;
}
