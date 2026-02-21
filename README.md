## voider 
by quik // unlicense

just project on c++.

What can he do?
    main options:
        -u, --uppercase     - convert to UPPERCASE
        -l, --lowercase     - convert to lowercase
        -c, --capitalize    - capitalize first letter
        -r, --reverse       - reverse the word
        -s, --separator <char> - separator between repetitions
        -p, --pattern <n>   - pattern (1-normal, 2-alternating)

    appearance effects:
        -d, --delay <ms>    - delay between characters
        -e, --effect <n>    - effect type:
                                0 - normal typewriter
                                1 - wave (with -w parameter)
                                2 - scanning
                                3 - blinking cursor
                                4 - reverse effect
                                5 - random reveal
        -w, --wave <size>   - wave size (for effect 1)
        -f, --fade          - fade out at the end

    visual effects:
        -b, --border        - add border
        -n, --number        - number lines
        -a, --rainbow       - rainbow text
        -g, --glitch <n>    - glitch effect (1-3 intensity)
        -m, --mirror        - mirror display
        -x, --matrix        - matrix style

    additional:
        -o, --output <file> - save to file
        -h, --help          - show this help

    examples:
        ./voider 3 4 void
        ./voider -d 50 -e 1 -w 3 3 3 hello
        ./voider -a -b -n 4 2 rainbow
        ./voider -d 30 -e 2 -f 3 3 matrix
        ./voider -g 2 -x 2 5 glitch

### to start:
```bash
g++ voider.cpp -o voider //for linux

x86_64-w64-mingw32-g++ -static -o voider.exe voider.cpp -std=c++11 //for windows
```
### or see [Releases](https://github.com/QUIK1001/voider/releases)