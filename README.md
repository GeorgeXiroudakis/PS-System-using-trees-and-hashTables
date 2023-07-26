### HY-240-project2
publish subscribe system using various forms of data trees and hash-table

## intro
this is a publish subscribe system using various forms of data trees(binary, leaf oriended ect) and and hash-table. There is a set number of publisers (groups),
then there are users that can subscribe/unsubscribe to a publiser and have access to the information they upload, the publishers can post information. 
finally the subcribers can consume the info from the publisers they are subscribed to after the info trees have been proumded.

## structure

# info structure
the main structure that shows stores the info of the groups and their subscribers is discribed in the diagram bellow that was given by the proffesor.
![Screenshot from 2023-07-26 20-29-09](https://github.com/GeorgeXiroudakis/HY-240-project2/assets/138579604/71334f9e-4f4e-4447-ae02-046e26b2881b)

# consume tree structure

the consume tree is a leaf oriended tree that stores the info before they are consumesed.
![Screenshot from 2023-07-26 20-44-03](https://github.com/GeorgeXiroudakis/HY-240-project2/assets/138579604/59f43927-0751-4908-ab1d-83b13d5544cb)

# subcribers hash-table
this hash-table stores the subcribers and their info. The hash-table is set up accoding to the commant-line arguments given (m numbers of chells in the hash-table,
p the prime number used in the hash function). 
![Screenshot from 2023-07-26 20-50-38](https://github.com/GeorgeXiroudakis/HY-240-project2/assets/138579604/909ee60a-74b6-435b-a17e-80b93a8a66ba)

## use
as mentioned in the subcribers hash-table section the first two commant line arguments are: m numbers of chells in the hash-table,
p the prime number used in the hash function.
then it takes with the instractions in a file in the format of the test files given. whete you first spesify the instractin (s for subrcibe, p print, c consum) and then the specifics of the instraction.
