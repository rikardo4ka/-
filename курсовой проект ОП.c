#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h> //заголовочные файлы
#include <locale.h>
#include <stdbool.h>
#define ROW 10 //определение чисел
#define COL 10

int writefilefix(const char* constFilefixName);//прототип функции для вывода игрового поля на экран из файла constfix.txt

const char* backupName = "constfix_backup.txt";
const char* constFilefixName = "constfix.txt";

int restoreBackup();//прототип функции возращения игрового поля в исходное состояние

int modcell(const char* constFilefixName);//прототип функции для записи значений в ячейки

int provfile(FILE* constFilefix, FILE* otvetFile);//прототип функции проверки путём сравнения файлов

int save(); //прототип диалога с пользователем по поводу сохранения

/*--------------главная функция, ----------------------------------------
---выполняет вышеперечисленные функции,вывод правил,меню и диалог с пользователем---*/

int main() {
    setlocale(LC_ALL, "RUS"); //функция для перевода на русский язык текста на консоли
    printf("Игра-головоломка Числобол\n");
    const char* constFilefixName = "constfix.txt";
    writefilefix(constFilefixName); //использование функции для вывода массива М на экран
    puts("\nДля решения головоломки нужно расставить числа из указанного\nдиапазона в определённых клетках сетки так,\nчтобы в каждой строке и в каждом столбце каждое число\nиспользовалось только один раз.Если в клетке числобола находится кружок,\nто она обязательно должна содержать число.Если в клетке стоит\nкрестик, то в ней числа быть не должно.");
    int num; //переменные для функции switch case
    int k = 0; //переменная для использования в цикле
    while (k < 2) {
        printf("\nКакое действие желаете выполнить?:\n1)занести значение\n2)показать текущее поле\n3)проверить\n4)показать ответ\n5)выйти из игры\n");
        scanf("%d", &num); //считывание значения
        switch (num) {
        case 1:
            modcell(constFilefixName);
            break;

        case 2:
            writefilefix(constFilefixName); //вывод текущего игрового поля на экран
            break;

        case 3:
            /*------------открытие файла constFilefix на режим чтения------------*/

            FILE* constFilefix = fopen("constfix.txt", "r");
            FILE* otvetFile = fopen("otvet.txt", "r");
            if (constFilefix == NULL || otvetFile == NULL) {
                printf("Ошибка при открытии файлов!\n");
                return -1;
            }
            provfile(constFilefix, otvetFile); 
            return 0;

        case 4:
            /*------------вывод массива O на экран------------*/
            printf("Ответ к головоломке Числобол:\n");
            const char* otvetFileName = "otvet.txt";
            writefilefix(otvetFileName);
            printf("Очень жаль что вы не справились сами...\n\n");
        case 5:
            save();
            return 0; // выход из игры

        default:
              /*--------сообщение выводится если ввели цифру,--------
            ------которая не входит в диапазон от 1 до 5---------*/
            puts("Введено неверное значение");

        } //switch num
    } // while (k < 2)
} // int main()

/*------------функция для вывода игрового поля на экран из файла constfix.txt-------------*/

int writefilefix(const char* constFilefixName) {
    FILE* constFilefix = fopen(constFilefixName, "r");
    if (constFilefix == NULL) {
        perror("Ошибка при открытии файлов!");
        return -1;
    }
    char c;
    while ((c = fgetc(constFilefix)) != EOF) {
        if (c != '\n') {
            printf("|_%c_|", c);
        }
        else {
            putchar(c);
        }
    }
    fclose(constFilefix);
    return 1;
}

/*------------функция возращения игрового поля в исходное состояние--------------------*/

int restoreBackup() {
    char ch;
    FILE* backupFile = fopen(backupName, "rb");
    FILE* constFilefix = fopen(constFilefixName, "wb");

    if (backupFile == NULL || constFilefix == NULL) {
        perror("Ошибка при открытии файлов!");
        return -1;
    }

    while ((ch = fgetc(backupFile)) != EOF) {
        fputc(ch, constFilefix);
    }

    fclose(constFilefix);
    fclose(backupFile);
    return 1;
}

/*------------функция для записи значений в ячейки-----------------------*/

int modcell(const char* constFilefixName) {
    FILE* constFilefix = fopen(constFilefixName, "r+");
    if (constFilefix == NULL) {
        perror("Ошибка при открытии файла!");
        return -1;
    }

    int mun;
    printf("Какую клетку вы хотите заменить?\nЗапишите двузначное число,где первая цифра будет означать строку,а вторая столбец:");
    scanf("%d", &mun);

    int i = mun / 10;
    int j = mun % 10;

    char M[10][10];
    fseek(constFilefix, i, SEEK_SET);
    fread(M, sizeof(char), sizeof(M), constFilefix);

    if (M[i][j] != 'X') {
        printf("Введите значение: ");
        char temp[2];
        scanf("%s", temp);
        M[i][j] = temp[0];

        fseek(constFilefix, i, SEEK_SET);
        fwrite(M, sizeof(char), sizeof(M), constFilefix);

        printf("Изменения приняты!\n");
    }
    else {
        printf("По правилам игры в эту клетку нельзя занести значение!\n");
    }

    fclose(constFilefix);
    return 1;
}

/*------------функция проверки путём сравнения файлов-----------------------*/

int provfile(FILE* constFilefix, FILE* otvetFile) {
    int num1, num2; //объявление переменных 
    int mistake = 0; //переменная для отслеживания несоответствия символов
    /*------------цикл while для осуществения проверки------------*/

    while (fscanf(constFilefix, "%c", &num1) == 1 &&
        fscanf(otvetFile, "%c", &num2) == 1) {
        if (num1 != num2) {
            mistake = 1;
            break;
        }
    }
    /*------------проверка условия несоответствия символов------------*/
    if (mistake == 1) { 
        printf("Головоломка решена неверно! Попробуйте в следующий раз.\n\n");
    }
    else {
        printf("Головоломка решена верно! Поздравляем!\n\n");
    }

    fclose(constFilefix); //закрытие файла constFilefix
    fclose(otvetFile); //закрытие файла otvetFile
    save();
    return 1;
}

/*------------диалог с пользователем по поводу сохранения-----------------------*/

int save() {
    int change;
    printf("Вы желаете сохраниться?\n1)да\n2)нет\n");
    scanf("%d", &change);
    if (change == 1) {
        return 0;
    }
    else if (change == 2) {
        restoreBackup();
    }
    return 0;
}
