//множество от семафори, представени като масив
//ако семафорът е 0, тогаво се подминава, а 1 е разрешено, като това може да се мени според, както ти е по-удобно

//semget - отваряме или създаваме семафор ; nsems - колко семафора искаме да има ; слагат се и права за достъп - четене и писане
// + setall за да се инициализират семафорите
//semop (int semid, struct * sops, unsigned nsops) - изпълнява операции върху семафори returns (1 - succ, -1 - fail)
//struct sembug
//(unsigned short sem_num(номер на сем), 
    //short sem_op(операция - 3 типа 
        //(привавяме (разрешаваме) > 0, забравяме 0, изваждаме (блокираме) < 0))
    //short sem_flg (2 основни които ползваме -> IPC_NOWAIT, SEM_UNDO) 
//)

//int semctl(int semid. int semnum, int cmd, union semun arg) - управлява семафор
//най-често semun ->  Null

//cmd
    //IPC_STAT - info за семафорите в argbuf
    //IPC_SET - променя инфото(собственик, права) за семафорите
    //IPC_RMID - destroy-ва семафори
    //GETALL - returns semval за всички сем в semun.array
    //GETVAL - returns semval за semnum (един сем)
    //Само при иниализация на сем, след това само чрез операции
    //SETALL - set-ва semval за всички сем
    //SETVAL - set-ва semval за semnum (един сем)

//struct semun 
    //int val - стойност
    //struct semid_ds *buf - буфер за IPC_STAT, IPC_SET
    //unsinged short * array - масив за GETALL и SETALL

//унищожаваме ipc обектите след като не се ползват повече
