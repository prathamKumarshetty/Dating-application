#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

class User {
public:
    string user_id;
    string name;
    string email;
    string gender;
    string password;
    int age;

    User(string userId, string name, string email, string gender, int age)
        : user_id(userId), name(name), email(email), gender(gender), age(age) {}

    virtual string getUserId() = 0;
    virtual string getName() = 0;
    virtual string getEmail() = 0;
    virtual string getGender() = 0;
    virtual string getPassword() = 0;
    virtual int getAge() = 0;
};

class Profile : public User {
public:
    string address;
    vector<string> interests;
    vector<Profile*> blocklist;
    vector<Profile*> matches;

    Profile(string userId, string name, string email, string gender, int age,
            string address, vector<string> interests)
        : User(userId, name, email, gender, age), address(address), interests(interests) {}

    string getUserId() override {
        return user_id;
    }

    string getName() override {
        return name;
    }

    string getEmail() override {
        return email;
    }

    string getGender() override {
        return gender;
    }

    string getPassword() override {
        return password;
    }

    int getAge() override {
        return age;
    }

    void receiveMessage(Profile* sender, const string& content) {
        cout << "Message received from " << sender->getName() << ": " << content << endl;
    }

    const vector<string>& getInterests() const { // Added getter method for interests
        return interests;
    }
};

class Chatroom {
public:
    vector<Profile*> participants;
    vector<string> messages; // Store messages as strings

    void addParticipant(Profile* participant) {
        participants.push_back(participant);
    }

    void removeParticipant(Profile* participant) {
        participants.erase(remove(participants.begin(), participants.end(), participant), participants.end());
    }

    void sendMessage(Profile* sender, Profile* receiver, const string& content) {
        if (find(participants.begin(), participants.end(), sender) == participants.end()) {
            cout << "Error: Sender not in the chatroom!" << endl;
            return;
        }

        if (find(participants.begin(), participants.end(), receiver) == participants.end()) {
            cout << "Error: Receiver not in the chatroom!" << endl;
            return;
        }

        // Construct the message string and store it in the messages vector
        string message = sender->getUserId() + " -> " + receiver->getUserId() + ": " + content;
        messages.push_back(message);

        if (sender != receiver) {
            receiver->receiveMessage(sender, content); // Pass the content directly to the receiver's receiveMessage function
        }
    }

    void displayMessages() const {
        for (const string& message : messages) {
            cout << message << endl;
        }
    }
};

class Matchmaker {
public:
    vector<Profile*> users;
    bool status;
    bool flag;

    void updateFlag(Profile* profile1, Profile* profile2) {
        flag = !users.empty();
        if (flag) {
            double score = compatibilityScore(profile1, profile2);
            status = (score > 0.5);
        } else {
            status = false;
        }
    }

    void notifyUsers() {
        if (status) {
            cout << "Congratulations! You have a new match!" << endl;
        } else {
            cout << "No matches found. Please wait for potential matches." << endl;
        }
    }

    Matchmaker() : status(false), flag(false) {}

    Profile* getUser(string userId) {
        for (Profile* profile : users) {
            if (profile->getUserId() == userId) {
                return profile;
            }
        }
        return nullptr;
    }

    void addUser(Profile* profile) {
        users.push_back(profile);
        for (Profile* existingProfile : users) {
            if (existingProfile != profile) {
                updateFlag(existingProfile, profile);
                updateFlag(profile, existingProfile);
            }
        }
        notifyUsers();
    }

    void removeUser(Profile* profile) {
        auto it = std::find(users.begin(), users.end(), profile);
        if (it != users.end()) {
            users.erase(it);
            for (Profile* existingProfile : users) {
                updateFlag(existingProfile, profile);
                updateFlag(profile, existingProfile);
            }
        }
        notifyUsers();
    }

    double compatibilityScore(Profile* profile1, Profile* profile2) {
        double score = 0.0;
        int numMatches = 0;
        for (const string& interest : profile1->getInterests()) {
            if (find(profile2->getInterests().begin(), profile2->getInterests().end(), interest) != profile2->getInterests().end()) {
                numMatches++;
            }
        }
        score = numMatches / 5.0;
        return score;
    }
};

void displayProfilesWithInterests(const vector<Profile*>& profiles) {
    cout << "--------------------------------------------------" << endl;
    cout << "List of Profiles with Interests:" << endl;
    for (Profile* profile : profiles) {
        cout << "User ID: " << setw(10) << profile->getUserId() << " | Interests: ";
        const vector<string>& interests = profile->getInterests();
        for (size_t i = 0; i < interests.size(); ++i) {
            cout << interests[i];
            if (i < interests.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
    cout << "--------------------------------------------------" << endl;
}

// Function to validate alphabetic names
bool isValidName(const string& name) {
    for (char ch : name) {
        if (!isalpha(ch) && ch != ' ') {
            return false;
        }
    }
    return true;
}

// Function to validate integer ages
bool isValidAge(const string& ageStr) {
    for (char ch : ageStr) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}

int main() {
    Matchmaker matchmaker;
    Chatroom chatroom;

    while (true) {
        cout << "Welcome to the Matchmaking App!" << endl;
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        try {
            switch (choice) {
                case 1: // User registration
                {
                    string userId, name, email, gender, address;
                    int age;
                    vector<string> interests;

                    cout << "Enter your user ID: ";
                    cin >> userId;

                    // Check if the user ID already exists and prompt for another if it does

                    cout << "Enter your name: ";
                    cin.ignore();
                    getline(cin, name);

                    // Validate name
                    if (!isValidName(name)) {
                        throw runtime_error("Error: Name should only contain alphabets.");
                    }

                    cout << "Enter your email: ";
                    cin >> email;

                    cout << "Enter your gender: ";
                    cin >> gender;

                    cout << "Enter your age: ";
                    string ageStr;
                    cin >> ageStr;

                    // Validate age
                    if (!isValidAge(ageStr)) {
                        throw runtime_error("Error: Age should be an integer.");
                    }
                    age = stoi(ageStr);

                    cout << "Enter your address: ";
                    cin.ignore();
                    getline(cin, address);

                    cout << "Enter your interests (separated by spaces): ";
                    string interestLine;
                    getline(cin, interestLine);

                    istringstream iss(interestLine);
                    string interest;
                    while (iss >> interest) {
                        interests.push_back(interest);
                    }

                    Profile* newProfile = new Profile(userId, name, email, gender, age, address, interests);
                    matchmaker.addUser(newProfile);

                    cout << "Registration successful!" << endl;
                }
                break;


             case 2: // User login
                {
                    string userIdOrEmail, password;
                    cout << "Enter your user ID or email: ";
                    cin >> userIdOrEmail;

                    Profile* currentUser = matchmaker.getUser(userIdOrEmail);
                    if (currentUser)
                     {

                        cout << "Enter your password: ";
                        cin >> password;

                    // Check if the password is correct

                    // Display user's details and options for matching and chatting

                    while (true) {
                        cout << "1. Find Matches" << endl;
                        cout << "2. Chat" << endl;
                        cout << "3. Logout" << endl;
                        cout << "Enter your choice: ";

                        int userChoice;
                        cin >> userChoice;

                        switch (userChoice) {
                            case 1: // Find Matches
                            {
                                vector<Profile*> matches;
                                for (Profile* profile : matchmaker.users) { // Use matchmaker.users instead of matchmaker.getAllUsers()
                                    if (profile != currentUser) {
                                        double score = matchmaker.compatibilityScore(currentUser, profile);
                                        if (score > 0.5) {
                                            matches.push_back(profile);
                                        }
                                    }
                                }

                                if (!matches.empty()) {
                                    displayProfilesWithInterests(matches);
                                } else {
                                    cout << "No matches found. Please wait for potential matches." << endl;
                                }
                            }
                            break;

                            case 2: // Chat with a match
                            {
                                vector<Profile*> matches;
                                for (Profile* profile : matchmaker.users) { // Use matchmaker.users instead of matchmaker.getAllUsers()
                                    if (profile != currentUser) {
                                        double score = matchmaker.compatibilityScore(currentUser, profile);
                                        if (score > 0.5) {
                                            matches.push_back(profile);
                                        }
                                    }
                                }

                                if (!matches.empty()) {
                                    displayProfilesWithInterests(matches);
                                    cout << "Enter the User ID of the profile you want to chat with: ";
                                    string chatUserId;
                                    cin >> chatUserId;

                                    Profile* chatUser = matchmaker.getUser(chatUserId);
                                    if (chatUser && find(matches.begin(), matches.end(), chatUser) != matches.end()) {
                                        // Add both users to the chatroom
                                        chatroom.addParticipant(currentUser);
                                        chatroom.addParticipant(chatUser);

                                        // Send a sample message from currentUser to chatUser
                                        string message = "Hello, this is " + currentUser->getName() + "!";
                                        chatroom.sendMessage(currentUser, chatUser, message);
                                    } else {
                                        cout << "Invalid User ID. Please try again." << endl;
                                    }
                                } else {
                                    cout << "No matches found. Please wait for potential matches." << endl;
                                }
                            }
                            break;

                            case 3:
                                cout << "Logging out..." << endl;
                                break;

                            default:
                                cout << "Invalid choice. Try again." << endl;
                                break;
                        }

                        if (userChoice == 3) {
                            break; // Break out of the inner loop to return to the main menu
                        }
                    }

                } else {
                        throw runtime_error("User not found. Please register or try again.");
                    }
                }
                break;


             case 3: // Exit
                    cout << "Goodbye! Exiting the application." << endl;
                    return 0;

                default:
                    throw runtime_error("Invalid choice. Try again.");
                    break;
            }
        } catch (const exception& e) {
            cerr << e.what() << endl;
        }

        // Clear any remaining characters in the input buffer before the next loop iteration
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return 0;
}
