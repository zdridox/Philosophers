*This project has been created as part of the 42 curriculum by mzdrodow*

# Description
Philosophers is a simulation project countering the good old philosophers siting at a round table problem.
The goal of this project is to learn about multithreaded programmes and managing resources between threads.
The problem is theres N philosophers sitting at a round table each one of them has to eat, sleep and think 
between every 2 philosophers theres a fork so theres also N forks on the table and one big bowl of spaghetti in the center.
Each philosopher requires 2 forks to Eat because obviously its not comfortable to eat spaghetti with only one but that means if one
philosopher is eating the philosophers next to him cannot eat because they lack forks after each philosopher eats he goes to sleep
after waking up he starts to think until he can get his hands on 2 forks to eat again if philosopher doesnt it in (TIME_TO_DEATH) time 
he will of course die this will stop the simulation philosophers of course should not die the simulation can also stop after reaching (number_of_times_each_philosopher_must_eat) which is a optional argument.

# Instructions
**Compilation**: </br>
    **make all** - compile the project. </br>
    **make clean** - clean the directory out of object files. </br>
    **make fclean** - clean the directory out of object files and the executable. </br>
    **make re** - recompile the project so fclean and after that all. </br>

**Usage**: </br>
**./philo** number_of_philosophers time_to_die time_to_eat time_to_sleep number_of_times_each_philosopher_must_eat(optional)

# Resources
Most of my knowledge and resources came from my Peers at 42Warsaw 
But i used AI for gathering knowledge about pthread library i asked it 
to give me examples and quick rundowns on mutexes and the main pthread functions
like create join etc.