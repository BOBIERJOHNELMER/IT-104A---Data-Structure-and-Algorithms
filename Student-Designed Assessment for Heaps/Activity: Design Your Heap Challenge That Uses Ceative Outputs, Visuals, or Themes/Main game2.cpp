#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

// Function to create text for displaying in the window
sf::Text createText(const string& content, const sf::Font& font, float x, float y, unsigned int size = 24) {
    sf::Text text;
    text.setFont(font);                // Set the font
    text.setString(content);           // Set the displayed content
    text.setCharacterSize(size);       // Set font size
    text.setFillColor(sf::Color::Green); // Set text color to black
    text.setPosition(x, y);            // Set position in the window
    return text;
}

// Function to load and scale a background image
sf::Sprite loadBackground(const string& filePath, sf::Texture& texture, float scaleX = 1.0f, float scaleY = 1.0f) {
    if (!texture.loadFromFile(filePath)) {
        cout << "Error: Could not load " << filePath << endl;
    }
    sf::Sprite sprite;
    sprite.setTexture(texture);       // Set the texture to the sprite
    sprite.setScale(scaleX, scaleY);  // Adjust the size of the sprite
    return sprite;
}

// Function to maintain heap property by "heapifying" from a node downwards
void heapify(vector<int>& heap, int n, int i) {
    int largest = i;                   // Assume the current node is the largest
    int left = 2 * i + 1;              // Index of left child
    int right = 2 * i + 2;             // Index of right child

    // Compare left child with the largest
    if (left < n && heap[left] > heap[largest])
        largest = left;

    // Compare right child with the largest
    if (right < n && heap[right] > heap[largest])
        largest = right;

    // If the largest is not the current node, swap and recurse
    if (largest != i) {
        swap(heap[i], heap[largest]);
        heapify(heap, n, largest);    // Heapify the affected subtree
    }
}

// Function to build a max-heap from the array
void buildHeap(vector<int>& heap) {
    int n = heap.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(heap, n, i);          // Heapify nodes in reverse order
    }
}

// Function to insert a new shipment
void insertShipment(vector<int>& heap, vector<int>& availableShipments, const sf::Font& font, const sf::Sprite& background) {
    sf::RenderWindow window(sf::VideoMode(785, 600), "Insert Shipment");
    sf::String userInput;
    sf::Text instructions = createText("\nEnter shipment value to insert: ", font, 50, 50);
    sf::Text availableText = createText("\nAvailable Shipments: ", font, 50, 100);
    sf::Text inputText = createText("", font, 50, 200);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)    // Close the window
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !userInput.isEmpty()) // Handle backspace
                    userInput.erase(userInput.getSize() - 1);
                else if (event.text.unicode == '\r') { // Handle Enter key
                    string input = userInput.toAnsiString();
                    try {
                        int value = stoi(input); // Convert string to integer
                        auto it = find(availableShipments.begin(), availableShipments.end(), value);
                        if (it != availableShipments.end()) {
                            heap.push_back(value); // Add to heap
                            buildHeap(heap);      // Rebuild heap
                            availableShipments.erase(it); // Remove from available shipments
                        }
                    }
                    catch (...) {
                        cout << "Invalid input. Please try again." << endl;
                    }
                    userInput.clear();            // Clear input after processing
                }
                else if (event.text.unicode < 128) {
                    userInput += event.text.unicode; // Append character to input
                }
            }
        }

        stringstream ss;
        ss << "\n\n\n\nAvailable Shipments: ";
        for (int val : availableShipments) ss << val << " ";
        availableText.setString(ss.str());         // Update available shipments display

        inputText.setString("\n\n\n\nInput: " + userInput.toAnsiString()); // Update input display

        window.clear();
        window.draw(background);                  // Draw background
        window.draw(instructions);
        window.draw(availableText);
        window.draw(inputText);
        window.display();
    }
}

// Function to dispatch the highest-priority shipment
void dispatchShipment(vector<int>& heap, const sf::Font& font, const sf::Sprite& background) {
    sf::RenderWindow window(sf::VideoMode(785, 600), "Dispatch Shipment");

    string message;
    if (!heap.empty()) {
        int top = heap.front();                   // Root of the heap
        heap[0] = heap.back();                    // Replace root with last element
        heap.pop_back();                          // Remove last element
        buildHeap(heap);                          // Rebuild heap
        message = "Dispatched shipment with priority:\n                             " + to_string(top);
    }
    else {
        message = "No shipments to dispatch!";
    }

    sf::Text messageText = createText(message, font, 50, 250);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)   // Close window
                window.close();
        }

        window.clear();
        window.draw(background);                  // Draw background
        window.draw(messageText);
        window.display();
    }
}

// Function to view the current heap
void viewHeap(const vector<int>& heap, const sf::Font& font, const sf::Sprite& background) {
    sf::RenderWindow window(sf::VideoMode(785, 600), "View Heap");

    stringstream ss;
    ss << "\n\nCurrent Heap: ";
    for (int val : heap) ss << val << " ";

    sf::Text heapText = createText(ss.str(), font, 50, 200);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)   // Close window
                window.close();
        }

        window.clear();
        window.draw(background);                  // Draw background
        window.draw(heapText);
        window.display();
    }
}

// Main function
int main() {
    vector<int> heap;                             // The max-heap
    vector<int> availableShipments = { 12, 35, 9, 18, 23 }; // Initial shipments

    sf::RenderWindow window(sf::VideoMode(785, 600), "Cargo Management");
    sf::Font font;
    if (!font.loadFromFile("SPACEBOY.ttf")) {        // Load font
        cout << "Error loading font!" << endl;
        return -1;
    }

    sf::Texture startupTexture, insertTexture, dispatchTexture, viewTexture;
    sf::Sprite startupBackground = loadBackground("startup.jpg", startupTexture, 0.5f, 0.6f); // Scale startup background
    sf::Sprite insertBackground = loadBackground("insert.png", insertTexture, 1.7f, 1.5f);    // Scale insert background
    sf::Sprite dispatchBackground = loadBackground("dispatch.png", dispatchTexture, 1.6f, 1.6f); // No scaling
    sf::Sprite viewBackground = loadBackground("view.png", viewTexture, 1.9f, 1.9f);          // Scale view background

    sf::Text menuText = createText(
        "1. Insert Shipment\n\n2. Dispatch Shipment\n\n3. View Heap ",
        font,
        350,
        150
    );
    sf::String userInput;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)   // Close window
                window.close();

            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == '\b' && !userInput.isEmpty()) // Backspace
                    userInput.erase(userInput.getSize() - 1);
                else if (event.text.unicode == '\r') { // Enter key
                    string input = userInput.toAnsiString();
                    if (input == "1") {
                        insertShipment(heap, availableShipments, font, insertBackground);
                    }
                    else if (input == "2") {
                        dispatchShipment(heap, font, dispatchBackground);
                    }
                    else if (input == "3") {
                        viewHeap(heap, font, viewBackground);
                    }
                    userInput.clear();            // Clear input after processing
                }
                else if (event.text.unicode < 128) {
                    userInput += event.text.unicode; // Append character
                }
            }
        }

        sf::Text inputText = createText("Enter number: " + userInput.toAnsiString(), font, 350, 500);

        window.clear();
        window.draw(startupBackground);
        window.draw(menuText);
        window.draw(inputText);
        window.display();
    }

    return 0;
}
