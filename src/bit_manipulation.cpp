#include <array>
#include <cstdio> // for printf
#include <cstdint> // for uint8_t, uint64_t etc.
#include <type_traits> // for is_integral
#include <bitset>
#include <iostream>

using namespace std;

// Create the bitmask for the K-th bit of the given value
// Usage: int i=13; int bit4_mask = make_bitmask<4>(i);
// Note: the value of the mask will be 2^K except for a 
// signed integer, the value of mask of bit N-1 is -2^(N-1)
template<size_t K, typename T>
T make_bitmask(T int_variable)
{
    static_assert(K < 8*sizeof(T), 
        "bit mask: No such bit position in given type");
    return T(1 << K);
}

// Create the bitset of the appropriate size from a given integer
// Usage: int i = 13; auto bi = as_binary(i); // bi will be of type bitset<32>
template<typename T>
bitset<8*sizeof(T)> as_binary(T int_value)
{
    static_assert(is_integral<T>::value, 
        "binary representation: non-integer types not supported ");
    return bitset<8*sizeof(T)>(int_value);
}

// C-style display base2 representation of an integer
// Note: works for both signed and unsigned
// Note: can be generalized to the other integer types
void print_binary(unsigned char int_value)
{
    for(unsigned char i = 1 << (8*sizeof(unsigned char)-1); i > 0; i >>= 1)
        (int_value & i) ? printf("1") : printf("0");
}

// C-style : keep only lowest set bit
// Note this is based on 1's complement: -n = ~n + 1
int lowest_set_bit(int int_value)
{
    return int_value & (-int_value);
}

// C-style : strip the lowest set bit
int strip_lowest_set_bit(int int_value)
{
    return int_value & (int_value-1);
}


void demo_bit_manipulation()
{
    cout << endl << "************* Bit manipulation *************" << endl;

    // Masks for a single bit in a 1-byte unsigned integer
    constexpr uint_fast8_t BIT0{ 1 << 0 }; // 0000 0001
    constexpr uint_fast8_t BIT1{ 1 << 1 }; // 0000 0010
    constexpr uint_fast8_t BIT2{ 1 << 2 }; // 0000 0100
    constexpr uint_fast8_t BIT3{ 1 << 3 }; // 0000 1000
    constexpr uint_fast8_t BIT4{ 1 << 4 }; // 0001 0000
    constexpr uint_fast8_t BIT5{ 1 << 5 }; // 0010 0000
    constexpr uint_fast8_t BIT6{ 1 << 6 }; // 0100 0000
    constexpr uint_fast8_t BIT7{ 1 << 7 }; // 1000 0000

    // Create number as a bitset
    auto number = bitset<8>("01101010");
    // Convert bitset to uint type
    uint_fast8_t int_number = static_cast<uint_fast8_t>(number.to_ulong());
    // Display as decimal and as base2
    // Note: uint_fast_8 has char as underlying type, so without the "+",
    // cout displays a character j which is the ASCII for 106
    cout << "Number: " << int_number << " = " << +int_number << " = 0b" << number << endl;
    // Combine single-bit masks
    auto bit_mask = BIT0 | BIT3 | BIT6;
    // Display resulting mask as decimal and as base2
    cout << "Bit mask: " << bit_mask << " = 0b" << bitset<8>(bit_mask) << endl;
    // Display as base2 using C-style
    printf("C-style : 106 = 0b"); print_binary(106); printf("\n");
    printf("C-style : -73 = 0b"); print_binary(-73); printf("\n");
    // Convert lower case to upper case - actually the AND mask just sets the 6th bit to zero
    char c1 = 'r';
    char lower_to_upper_mask = 0xDF; // 0b1101'1111
    cout << c1 << " & 0xDF  = " << char(c1 & lower_to_upper_mask) << endl;
    cout << bitset<8>(c1) << " & " << bitset<8>(lower_to_upper_mask) << " = " << bitset<8>(c1 & lower_to_upper_mask) << endl;
    // Convert upper case to lower case - use the complemented OR mask to set the 6th bit to one
    char c2 = 'R';
    char upper_to_lower_mask = 1 << 5; // 0b0010'0000
    cout << c2 << " | 0x" << hex << +upper_to_lower_mask << dec << " = " << char(c2 | upper_to_lower_mask) << endl;
    cout << bitset<8>(c2) << " | " << bitset<8>(upper_to_lower_mask) << " = " << bitset<8>(c2 | upper_to_lower_mask) << endl;

    char c3 = 'a';
    short s3 = 257;
    short s4 = -s3;
    int i3 = 1E5;
    // as_binary converts integer to the corresponding bitset - required for binary representation
    cout << c3 << " = " << as_binary(c3) << endl;
    cout << s3 << " = " << as_binary(s3) << endl;
    cout << s4 << " = " << as_binary(s4) << endl;
    cout << i3 << " = " << as_binary(i3) << endl;
    // Does not compile: not an integral type
    // float f3 = 3.14;
    // cout << f3 << " = " << as_binary(f3) << endl;

    // make_bitmask
    cout << "0th bit mask for a char  : " << as_binary(make_bitmask<0>('r')) << endl;
    cout << "5th bit mask for a short : " << as_binary(make_bitmask<5>(s4)) << endl;
    cout << "3rd bit mask for a int   : " << as_binary(make_bitmask<3>(int())) << endl;
    // Does not compile: 8th bit of char does not exist
    // cout << "8th bit mask for a char  : " << as_binary(make_bitmask<8>('r')) << endl;

    // **** C-style manipulations (integer types) ****
    cout << endl << "*** C-style bit manipulation ***" << endl;
    cout << "Number        : " << +int_number << "\t= " << as_binary(int_number) << endl;
    int_number |= BIT2;
    cout << "Set bit 2     : " << +int_number << "\t= " << as_binary(int_number) << endl;
    int_number &= ~BIT5;
    cout << "Cleared bit 5 : " << +int_number << "\t= " << as_binary(int_number) << endl;
    int_number ^= BIT7;
    cout << "Flipped bit 7 : " << +int_number << "\t= " << as_binary(int_number) << endl;
    bool is_bit1_set = int_number & BIT1;
    cout << "Is bit1 set ? : " << (is_bit1_set ? "true" : "false") << endl;
    bool is_bit4_set = (int_number >> 4 ) & 1;
    cout << "Is bit4 set ? : " << (is_bit4_set ? "true" : "false") << endl;
    // This next trick works if x is either 0 or 1
    int x = 1;
    int_number ^= (-x ^ int_number) & BIT4;
    cout << "Set bit 4 to x=" << x << " : " << +int_number << "\t= " << as_binary(int_number) << endl;
    int_number ^= (-x ^ int_number) & BIT4;
    cout << "Set bit 4 to x=" << x << " : " << +int_number << "\t= " << as_binary(int_number) << endl;
    x = 0;
    int_number ^= (-x ^ int_number) & BIT4;
    cout << "Set bit 4 to x=" << x << " : " << +int_number << "\t= " << as_binary(int_number) << endl;
    int_number ^= (-x ^ int_number) & BIT4;
    cout << "Set bit 4 to x=" << x << " : " << +int_number << "\t= " << as_binary(int_number) << endl;

    uint bit_count = 0;
    for (uint_fast8_t int_copy = int_number; int_copy; ++bit_count)
    {
        int_copy &= int_copy - 1;
    }
    cout << "Number of set bits            : " << bit_count << endl;

    unsigned int u1 = 256u;
    unsigned int is_power_of_2 = u1 && !(u1 & (u1 - 1));
    cout << u1 << " is a power of two ?    : " << (is_power_of_2 ? "true" : "false") << endl;
    char c5 = -67;
    #define IS_POWER_OF_TWO(x) (x && !(x & (x-1)))
    cout << +c5 << " is a power of two ?    : " << (IS_POWER_OF_TWO(c5) ? "true" : "false") << endl;
    #undef IS_POWER_OF_TWO
    
    int_number = -1;
    cout << "Set all bits (uint_fast8_t)   : " << +int_number << "\t= " << as_binary(int_number) << endl;
    int_number = 0;
    cout << "Clear all bits (uint_fast8_t) : " << +int_number << "\t= " << as_binary(int_number) << endl;
    unsigned short s5 = -1;
    cout << "Set all bits (unsigned short) : " << +s5 << "\t= " << as_binary(s5) << endl;
    int i4 = ~int(0);
    cout << "Set all bits (signed int)     : " << +i4 << "\t= " << as_binary(i4) << endl;
    // Note: the -1 technique works for signed and unsigned and is more portable than
    // int_type x = ~int_type(0)

    // **** C++-style manipulations (bitest) ****
    cout << endl << "*** C++-style bit manipulation ***" << endl;
    cout << "Number        : " << number.to_ulong() << "\t= " << number << endl;
    number.set(2);
    cout << "Set bit 2     : " << number.to_ulong() << "\t= " << number << endl;
    number.reset(5);
    cout << "Cleared bit 5 : " << number.to_ulong() << "\t= " << number << endl;
    number.flip(7);
    cout << "Flipped bit 7 : " << number.to_ulong() << "\t= " << number << endl;
    is_bit1_set = number.test(1);
    cout << "Is bit1 set ? : " << (is_bit1_set ? "true" : "false") << endl;
    is_bit4_set = number.test(4);
    cout << "Is bit4 set ? : " << (is_bit4_set ? "true" : "false") << endl;
    cout << "Number of set bits            : " << number.count() << endl;
    // This next trick works if x is either 0 or 1
    x = 1;
    number.set(4, x > 0);
    cout << "Set bit 4 to x=" << x << " : " << number.to_ulong() << "\t= " << number << endl;
    number[4] = (x > 0);
    cout << "Set bit 4 to x=" << x << " : " << number.to_ulong() << "\t= " << number << endl;
    x = 0;
    number.set(4, x > 0);
    cout << "Set bit 4 to x=" << x << " : " << number.to_ulong() << "\t= " << number << endl;
    number[4] = (x > 0);
    cout << "Set bit 4 to x=" << x << " : " << number.to_ulong() << "\t= " << number << endl;
    auto bu1 = as_binary(u1);
    cout << u1 << " is a power of two ?    : " << (bu1.count() == 1 ? "true" : "false") << endl;
    auto bc5 = as_binary(c5);
    cout << +c5 << " is a power of two ?    : " << (bc5.count() == 1 ? "true" : "false") << endl;
    number.set();
    cout << "Set all bits (bitset<8>)      : " << number.to_ulong() << "\t= " << number << endl;
    number.reset();
    cout << "Clear all bits (bitset<8>)    : " << number.to_ulong() << "\t= " << number << endl;
    auto us6 = ushort(-1);
    cout << "Set all bits (unsigned short) : " << +us6 << "\t= " << as_binary(us6) << endl;
    auto i6 = ~int(0);
    cout << "Set all bits (signed int)     : " << +i6 << "\t= " << as_binary(i6) << endl;

    cout << endl << "*** Byte masks ***" << endl;
    // Mask for a single byte inside an int
    typedef bitset<8*sizeof(int)> int_bitmask;
    array<int_bitmask, sizeof(int)> BYTE_MASKS;
    for(size_t i = 0; i<sizeof(int); ++i)
    {
        BYTE_MASKS[i] = int_bitmask(0xff << i*8);
        cout << "Byte " << i << " : " << BYTE_MASKS[i] << endl;
    }

    cout << endl << "*** C-style bit hacks ***" << endl;
    char nb=84;
    cout << "Number               : " << +nb << " = " << as_binary(nb) << endl;
    char lowest_bit_only = lowest_set_bit(nb);
    cout << "Keep only lowest bit : " << +lowest_bit_only << " = " << as_binary(lowest_bit_only) << endl;
    char lowest_bit_striped = strip_lowest_set_bit(nb);
    cout << "Strip lowest bit     : " << +lowest_bit_striped << " = " << as_binary(lowest_bit_striped) << endl;
}
