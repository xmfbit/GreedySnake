objs = snake.o greedysnake.o 
snake:$(objs)
	@gcc -o snake $(objs) -lncurses
	@echo "Complete!\n"

snake.o:snake.h 

greedysnake.o:snake.h  

.PHONY:clean
clean:
	-rm snake $(objs) 

