#include<iostream>
#include<vector>
#include<unordered_map>
#include<sstream>
#include<memory>

class Environment {
private:
    std::unordered_map<std::string, int> storage;
public:
    std::shared_ptr<Environment> outer;

    Environment() : outer(nullptr) {}

    int get(const std::string& name) {
        auto it = storage.find(name);

        if (it != storage.end()) {
            return it->second;
        }

        if (outer != nullptr) {
            return outer->get(name);
        }

        return 0;
    }

    void set(const std::string& name, int val) {
        storage[name] = val;
    }
};

std::shared_ptr<Environment> NewEnv() {
    return std::make_shared<Environment>();
}

std::shared_ptr<Environment> NewEnclosedEnv(std::shared_ptr<Environment> outer) {
    auto env = std::make_shared<Environment>();
    env->outer = outer;
    return env;
}

int evalExpr(const std::string& expr, std::shared_ptr<Environment> env) {
    std::istringstream iss(expr);
    int val;

    if (iss >> val) {
        char remaining;
        if (!(iss >> remaining)) {
            return val;
        }
    }

    return env->get(expr);
}

int parseLet(std::vector<std::string>& words, std::shared_ptr<Environment> env) {
    if (words.size() < 4) {
        std::cout << "Error: Invalid let statement\n";
    }

    words.erase(words.begin()); // let is removed

    std::string var = words[0];
    words.erase(words.begin()); // var is removed

    if (!words.empty() && words[0] == "=") {
        words.erase(words.begin()); // = is removed
    }

    if (words.empty()) {
        std::cout << "Error: nothing to bind\n";
    }

    std::string expr = words[0];
    int val = evalExpr(expr, env);
    env->set(var, val);

    return val;
}

int parseIdent(const std::string& ident, std::shared_ptr<Environment> env) {
    return env->get(ident);
}

int scopeCount = 0;
int Eval(const std::string& line, std::shared_ptr<Environment>& currentEnv) {
    std::vector<std::string> words;
    std::istringstream iss(line);
    std::string word;

    while (iss >> word) {
        words.push_back(word);
    }

    if (words.empty()) {
        return 0;
    }

    if (words[0] == "let") {
        return parseLet(words, currentEnv);
    } else if (words[0] == "deep") {
        currentEnv = NewEnclosedEnv(currentEnv);
        ++scopeCount;
        std::cout << scopeCount << ": Scope Deep!\n";
        return 0;
    } else if (words[0] == "shallow") {
        if (currentEnv->outer != nullptr) {
            currentEnv = currentEnv->outer;
            std::cout << "Out Of Scope " << scopeCount << "\n";
            --scopeCount;
        } else {
            std::cout << "Currently In Global Scope\n";
        }
        return 0;
    } else {
        return parseIdent(words[0], currentEnv);
    }

    return 0;
}

const std::string Prompt = ">> ";

void Start(std::istream& in, std::ostream& out) {
    std::string line;
    auto currentEnv = NewEnv();

    while (true) {
        out << Prompt;
        out.flush();

        if (!std::getline(in, line)) {
            return;
        }

        if (line == "exit") {
            return;
        }

        int result = Eval(line, currentEnv);
        if (result != 0 || (line != "deep" && line != "shallow")) {
            out << result << "\n";
        }
    }
}

int main() {
    Start(std::cin, std::cout);

    std::cin.get();
}
