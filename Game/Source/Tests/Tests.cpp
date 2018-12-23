#include "Tests/Tests.h"

#include "Util/Crc32.h"

#include "Memory/MemoryTests.h"
#include "Logger.h"

#include <cstring>


namespace Tests
{

bool Crc32Test()
{
    bool success = true;

    static constexpr char* kTestStrings[] =
    {
        "Kelly Washington",
        "Maria Diaz",
        "Peter Stewart",
        "Andrea Sanders",
        "Tammy Nelson",
        "Kathleen Parker",
        "Catherine Griffin",
        "William Lopez",
        "Frank Jenkins",
        "Edward Howard",
        "Howard Russell",
        "Laura Johnson",
        "Evelyn Simmons",
        "Marie Gonzales",
        "Todd Robinson",
        "Carl Clark",
        "Rebecca Allen",
        "Willie Jones",
        "Doris Flores",
        "Keith Carter",
        "Ann Martinez",
        "Sandra Cook",
        "Bruce Green",
        "Victor Richardson",
        "Julie Morgan",
        "Lois Kelly",
        "Donna Foster",
        "Kevin Roberts",
        "Katherine Phillips",
        "Thomas Moore",
        "Stephen Walker",
        "Helen Hernandez",
        "Diane Powell",
        "Melissa Evans",
        "Sharon Gonzalez",
        "Wanda White",
        "Robert Lewis",
        "Brenda Price",
        "Kathryn Harris",
        "Fred Rodriguez",
        "Martin Peterson",
        "Jane Butler",
        "Pamela Barnes",
        "Judith Brown",
        "Jose Alexander",
        "Sara Scott",
        "Irene Williams",
        "Matthew Wood",
        "Debra Thomas",
        "Joe Perez"
    };
    static constexpr u32 kTestStringsCount = sizeof(kTestStrings) / sizeof(*kTestStrings);

    u32 hashes[kTestStringsCount];

    for (int i = 0; i < kTestStringsCount; ++i)
    {
        const char* pStr = kTestStrings[i];
        hashes[i] = Crc32(pStr, strlen(pStr));
    }

    // Check for collisions
    for (int i = 0; i < (kTestStringsCount - 1); ++i)
    {
        for (int j = (i + 1); j < kTestStringsCount; ++j)
        {
            if (hashes[i] == hashes[j])
            {
                LOGI("Crc32 clash found between %s and %s!", kTestStrings[i], kTestStrings[j]);
                success = false;
            }
        }
    }

    // Create another list and check the hashes are contained in the table
    // Order has been randomized
    static constexpr char* kTestStringsRand[] =
    {
        "Melissa Evans",
        "Katherine Phillips",
        "Donna Foster",
        "Judith Brown",
        "Carl Clark",
        "Brenda Price",
        "Jane Butler",
        "Andrea Sanders",
        "Stephen Walker",
        "Catherine Griffin",
        "Fred Rodriguez",
        "Marie Gonzales",
        "Lois Kelly",
        "Victor Richardson",
        "Bruce Green",
        "Maria Diaz",
        "Helen Hernandez",
        "Julie Morgan",
        "Doris Flores",
        "Matthew Wood",
        "Evelyn Simmons",
        "Irene Williams",
        "Sharon Gonzalez",
        "Kelly Washington",
        "Kevin Roberts",
        "Joe Perez",
        "Debra Thomas",
        "Laura Johnson",
        "Kathleen Parker",
        "Rebecca Allen",
        "William Lopez",
        "Wanda White",
        "Todd Robinson",
        "Willie Jones",
        "Thomas Moore",
        "Sandra Cook",
        "Sara Scott",
        "Robert Lewis",
        "Diane Powell",
        "Edward Howard",
        "Pamela Barnes",
        "Howard Russell",
        "Peter Stewart",
        "Martin Peterson",
        "Frank Jenkins",
        "Keith Carter",
        "Jose Alexander",
        "Ann Martinez",
        "Tammy Nelson",
        "Kathryn Harris",
    };
    static constexpr u32 kTestStringsRandCount = sizeof(kTestStringsRand) / sizeof(*kTestStringsRand);

    for (int i = 0; i < kTestStringsRandCount; ++i)
    {
        const char* pStr = kTestStringsRand[i];
        u32 hash = Crc32(pStr, strlen(pStr));
        bool found = false;
        for (int j = 0; j < kTestStringsCount; ++j)
        {
            if (hash == hashes[j])
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            LOGI("Unable to find %s in table! h1=0x(%x) h2=0x(%x)", kTestStringsRand[i], hash, hashes[i]);
            success = false;
        }
    }

    // Ensure that another string hash isn't in the list
    static constexpr char* kTestStringsExcluded[] =
    {
        "Amanda Perez",
        "Harold Campbell",
        "Jack Russell",
        "Ernest Robinson",
        "Carolyn Roberts",
        "Janet Brown",
        "Keith Green",
        "Sean Barnes",
        "Craig Smith",
        "Lori Hughes",
        "Judith Rivera",
        "Nicholas Turner",
        "Lois Wilson"
    };
    static constexpr u32 kTestStringsExcludedCount = sizeof(kTestStringsExcluded) / sizeof(*kTestStringsExcluded);

    for (int i = 0; i < kTestStringsExcludedCount; ++i)
    {
        const char* pStr = kTestStringsExcluded[i];
        u32 hash = Crc32(pStr, strlen(pStr));
        bool found = false;
        for (int j = 0; j < kTestStringsCount; ++j)
        {
            if (hash == hashes[j])
            {
                LOGI("Found clash between %s 0x(%x) and %s 0x(%x)!", kTestStrings[i], hash, kTestStrings[i], hashes[i]);
                success = false;
                found = true;
                break;
            }
        }
    }

    return success;
}


bool Do()
{
    bool result;

    result = Momo::Memory::PerformTests();
    ASSERT(result);

    result ^= Crc32Test();

    return result;
}

}