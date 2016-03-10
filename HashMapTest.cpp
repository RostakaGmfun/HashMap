#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "HashMap.hpp"
#include <vector>


TEST_CASE( "LinkedList Test", "[LinkedList]") {
    SECTION("Basic test") {
        LinkedList<int> list;

        REQUIRE( list.size() == 0 );
        list.clear();
        REQUIRE( list.size() == 0 );
        
        const int numElem = 10000;
        std::string expected;
        int err = 0;
        for(int i = 0;i<numElem;i++) {
            expected+=std::to_string(i);
            list.pushBack(i);
            if(list.size() != i+1) {
                err++;
            }
        }

        CHECK( err == 0 );

        std::string got;
        for(int i = 0;i<numElem;i++) {
            got+=std::to_string(list[i]);
        }

        REQUIRE( got == expected );

        list.clear();
        REQUIRE (list.size() == 0);
    }
}

TEST_CASE("HashMap Test", "[HashMap]") {
    SECTION("hash() Test") {
        const int numHashes = 1000;
        std::vector<std::size_t> hashes(numHashes);
        int err = 0;

        // test hash for std::string
        const int numChars = 20;
        for(int i = 0;i<numHashes;i++) {
            std::string str;
            for(int j = 0;j<numChars;j++) {
                str.push_back('0'+std::rand()%('z'-'0'));
            }
            std::size_t h = hash(str);
            if(std::find(hashes.begin(), hashes.end(), h) != hashes.end()) {
                err++;
            }
            hashes[i] = h;
        }
        CHECK( err == 0 );

        // test hash for std::int32_t
        hashes.clear();
        err = 0;
        for(int i = 0;i<numHashes;i++) {
            std::size_t h = hash(std::int32_t(i));
            if(std::find(hashes.begin(), hashes.end(), h) != hashes.end()) {
                err++;
            }
            hashes[i] = h;
        }
        CHECK( err == 0);

    }

    SECTION("HashMap basic Test") {
        HashMap<std::string, std::string> dict;

        REQUIRE( dict.size() == 0 );
        REQUIRE( dict.capacity()>0 );

        const int numElem = 1000;

        std::string expected;
        for(int i = 0;i<numElem;i++) {
            dict["key"+std::to_string(i)] = "value"+std::to_string(i);
            expected+="value"+std::to_string(i);
        }

        std::string got;
        for(int i = 0;i<numElem;i++) {
            got+=dict["key"+std::to_string(i)];
        }

        REQUIRE( got.size() == expected.size() );
    }
}
