#include "GeneratorToken.h"

string GeneratorToken::generateToken(size_t lenght)
{
    const string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    random_device rb;
    mt19937 generator(rb());
    uniform_int_distribution<> distribution(0, characters.size() - 1);
    string token;
    for (size_t i = 0; i < lenght; i++)
    {
        token += characters[distribution(generator)];
    }
    return token;
}
