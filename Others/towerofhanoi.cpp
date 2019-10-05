#include <iostream>
#include <vector>

void print(std::string s, std::vector<int>& v){
    std::cout << s << '\n';
    for (int i : v){
        std::cout << i << '\t';
    }
    std::cout << '\n';
    //print function that takes a string name and prints it followed by each of the elements of the vector followed by a tab

}

void move(int n, std::vector<int>& source, std::vector<int>& target, std::vector<int>& spare){
    // void function, does not return anything
    // n: number of disks
    // source: peg where the disks start
    // target: peg where the disks end
    // spare: peg which will be empty at the end
    // call by reference is used "std::vector<int>&" not call by value "std::vector<int>" to prevent memory faults
    static int move_counter = 0;
    // counter initialized to count number of steps to completion, static to prevent reinitialization at every move function call
    if (n > 0) {

        move(n-1, source,spare,target);


        target.push_back(source.back());
        // source.back() returns final value of source
        // target.push_back(x) adds x as a new value on the end of target
        source.pop_back();
        // removes final value of source to prevent two disks of the same value

        std::cout << "move: " << ++move_counter  << '\n';
        // counts total moves and prints
        
        print("Peg1",source);
        print("Peg2",spare);
        print("Peg3",target);
        // prints peg values

     

        move(n-1,spare,target,source);

    }
    return;

    // the actual recursion is difficult to explain, basically the tower of hanoi uses 3 pegs, a source where all the pegs start, a spare where nothing ends up and a target
    // where all the pegs go. Each recursion effectively moves all the disks onto the spare peg apart from the final one, once this has happened this final disk is moved to the
    // target, to the other disks this target peg is "effectively" blank as they are free to move on and off it. Once this has occured we have a new tower of hanoi problem but our
    // original spare is now our source and the original source is now the spare. The first call to move in the move call does that firs iteration, moving the top disks out the way,
    // the second call to move in the move call resets and does the tower of hanoi again but has swapped the target and auxillary pegs. This swapping keeps happening until there are
    // no moves left.

}



int main()
{
    std::vector<int> Peg1{3,2,1};
    std::vector<int> Peg2;
    std::vector<int> Peg3;

    //3 pegs initialized as vectors of integers
    //Peg1 has 3 "disks" the bottom one Peg1[0] being largest

    move(Peg1.size(),Peg1,Peg3,Peg2);

    //call of move, recursive so only needs to be called once
    //Peg1.size() returns length of Peg1, the number of "disks"
}
