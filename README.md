# SmallShooter
SDL space shooter 

For this project, I focused on understanding and creating the entity system rather than optimizing access to the data. I have created EntityManager that deals with all the needed operations as updating, rendering, colliding, setting and storing arrays of component data. All the data is processed array by the array for optimized memory access. 
Currently, there can be stutters with a high amount of entities, but for space-shooter gameplay purposes, it can handle the needed amounts without issues. The system is not final and could include optimizations for sorting data, finding data, grouping up the components into structs and many better optimized features. 
