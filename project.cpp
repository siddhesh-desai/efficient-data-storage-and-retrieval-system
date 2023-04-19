#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int member_number = 0;
int trainer_number = 0;
int slot_number = 0;
int covid_protocol[15];

typedef struct Node
{
    int member_ID;
    char name[20];
    int age;
    char gender[10];
    char program[20];
    int time_preffered;
    int trainer_pref_id;
    int pref_trainer_assign_index;
    int slot_booking_index;
    struct Node *next;
} member;

typedef struct trainer_node
{
    int trainer_ID;
    char name[20];
    int age;
    int renumeration;
    char program[20];
    struct trainer_node *next;
    int slots_booked[15]; // 6 am to 9 pm we get 14 hours so accordingly slot booking
    int availability;
} trainer;

struct slot_booked
{
    int member_id;
    int trainer_id;
    int time;

    struct slot_booked *next;
};

void Add_member(struct Node **head_ref)
{
    struct Node *new_member = (struct Node *)malloc(sizeof(struct Node));
    struct Node *last = *head_ref;

    member_number++;
    new_member->member_ID = member_number + 1000; // Assigning random member id
    printf("Name :");
    scanf("%s", &new_member->name);
    printf("Age :");
    scanf("%d", &new_member->age);
    printf("Gender(MALE/FEMALE/OTHER):");
    scanf("%s", &new_member->gender);
    printf("Program(YOGA/CARDIO/ZUMBA/WEIGHT LIFTING):");
    scanf("%s", &new_member->program);
    new_member->next = NULL;

    printf("________________________________________________________________________________________________________");
    printf("\nMember_Id:%d\nName :%s\nAge:%d\nGender:%s\nProgram Enrolled :%s ", new_member->member_ID, new_member->name, new_member->age, new_member->gender, new_member->program);

    if (*head_ref == NULL)
    {
        *head_ref = new_member;
        printf("\n\t\t The member has been added successfully");
        return;
    }

    while (last->next != NULL) // Traversing through the list
        last = last->next;

    last->next = new_member; // Append new member
    printf("\n\t\t The new member has been added successfully");
    return;
}

void Add_trainer(struct trainer_node **head_ref)
{
    struct trainer_node *new_trainer = (struct trainer_node *)malloc(sizeof(struct trainer_node));
    struct trainer_node *last = *head_ref;

    trainer_number++;
    new_trainer->trainer_ID = trainer_number + 5000; // Assigning random trainer id
    printf("Name :");
    scanf("%s", &new_trainer->name);
    printf("Age :");
    scanf("%d", &new_trainer->age);
    printf("Renumeration:");
    scanf("%d", &new_trainer->renumeration);
    printf("Program teaching(YOGA/CARDIO/ZUMBA/WEIGHT LIFTING)(If multiple , write with ','):");
    scanf("%s", &new_trainer->program);
    new_trainer->next = NULL;

    printf("________________________________________________________________________________________________________");
    printf("\nTrainer_Id:%d\nName :%s\nAge:%d\nRemuneration:%d\nProgram Teaching :%s ", new_trainer->trainer_ID, new_trainer->name, new_trainer->age, new_trainer->renumeration, new_trainer->program);

    if (*head_ref == NULL)
    {
        *head_ref = new_trainer;
        printf("\n\t\t The trainer has been added successfully");
        return;
    }

    while (last->next != NULL) // Traversing through the list
        last = last->next;

    last->next = new_trainer; // Append new trainer

    printf("\n\t\t The new trainer has been added successfully");
    printf("________________________________________________________________________________________________________");
    return;
}

void slot_push(struct slot_booked **head_ref, struct Node *member_node, struct trainer_node *trainer_node)
{
    struct slot_booked *new_booking = (struct slot_booked *)malloc(sizeof(struct slot_booked));

    new_booking->member_id = member_node->member_ID; // transfering the data
    new_booking->trainer_id = trainer_node->trainer_ID;
    new_booking->time = member_node->time_preffered;

    new_booking->next = *head_ref; // pushing new node
    *head_ref = new_booking;
}

/*int check_timestamp(char prev_time[],char curr_time[])
{
    int i=0,count=0;
    while(i<11)
    {
        if(prev_time[i]==curr_time[i])
        {
            count++;
        }
    }
    if(count==10)
        return 1;
    else
         return 0;
}*/

void print_non_assigned_mem(struct Node **head_ref)
{
    struct Node *lptr = *head_ref;
    printf("Non - assigned member list : \n");
    while (lptr != NULL)
    {
        if (lptr->slot_booking_index != 1)
        {
            printf("\n_______________________________________________________________________________________________\n");
            printf("\nMember_Id:%d\nName :%s\nAge:%d\nGender:%s\nProgram Enrolled :%s \n", lptr->member_ID, lptr->name, lptr->age, lptr->gender, lptr->program);
            printf("_______________________________________________________________________________________________\n");
            lptr = lptr->next;
        }
        else
            lptr = lptr->next;
    }
}
void schedule_slot(struct slot_booked **slot_head, struct Node **head_ref, struct trainer_node **head_trainer)
{
    struct Node *lptr = *head_ref;
    struct trainer_node *tptr = *head_trainer;
    int flag = 1;

    int member_id, time_pref, trainer_pref;
    char time_stamp[30];
    if (lptr == NULL)
    {
        printf("No member found . Add a member first. ");
    }
    else if (tptr == NULL)
    {
        printf("No trainer found . Add a trainer first. ");
    }
    else
    {
        int member_id;
        printf("Enter your member id :");
        scanf("%d", &member_id);

        while (lptr != NULL && flag == 1)
        {
            if (lptr->member_ID == member_id)
                flag = 0;
            else
                lptr = lptr->next;
        }
        if (lptr == NULL)
        {
            printf("No member with given id found . Try again ");
            return;
        }

        int correct = 0;
        while (correct == 0)
        {
            printf("Enter your time preference (1hr slot 6am-8pm ) for ex 6 if 6 am , 14 if 2 pm:");
            scanf("%d", &time_pref);
            if (time_pref > 20 || time_pref < 6)
            {
                printf("Enter a time between 6:00-20:00");
            }
            else
                correct = 1;
        }

        printf("Enter your preffered trainer id:");
        scanf("%d", &trainer_pref);

        lptr->time_preffered = time_pref;
        lptr->trainer_pref_id = trainer_pref;

        time_t rawtime; // for getting time stamp of booking
        struct tm *timeinfo;

        int flag2 = 1;
        int status = 0;

        while (tptr != NULL && flag2 == 1)
        {
            if (trainer_pref == tptr->trainer_ID)
            {
                if (strstr(tptr->program, lptr->program) != NULL || strcmp(tptr->program, lptr->program) == 0) // CHECK PROGRAM
                {
                    if (covid_protocol[(time_pref - 6)] > 10) // COVID PROTOCOL
                    {
                        printf("\nBookings at this time exceeded the covid protocol");
                        flag2 = 0;
                    }
                    else if ((tptr->slots_booked[(time_pref - 6)]) != 1 && flag2 == 1) // SLOT EMPTY CHECK
                    {
                        slot_push(slot_head, lptr, tptr);

                        tptr->slots_booked[time_pref - 6] = 1;
                        time(&rawtime);
                        timeinfo = localtime(&rawtime);

                        strcpy(time_stamp, asctime(timeinfo));

                        printf("\nYour slot has been successsfully booked at %s", time_stamp);
                        printf("_______________________________________________________________________________________________\n");
                        printf("\nMember_Id:%d\nTrainer_Id:%d\nName :%s\nAge:%d\nRemuneration:%d\nProgram Teaching :%s ", lptr->member_ID, tptr->trainer_ID, tptr->name, tptr->age, tptr->renumeration, tptr->program);
                        printf("\nOperating time slots of trainer are: ");
                        for (int i = 0; i < 15; i++)
                        {
                            if (tptr->slots_booked[i] == 1)
                                printf("[%d]", i + 6);
                        }

                        printf("_______________________________________________________________________________________________\n");

                        covid_protocol[(time_pref - 6)]++;
                        status = 1;
                        lptr->pref_trainer_assign_index = 1;
                        lptr->slot_booking_index = 1;
                        // strcpy(lptr->time_stamp,time_stamp);
                        flag2 = 0;

                        print_non_assigned_mem(head_ref);
                    }
                }
            }

            else
                tptr = tptr->next;
        }
        if (status == 0)
        {
            flag2 = 1;
            tptr = *head_trainer;
            while (tptr != NULL && flag2 == 1 && status == 0)
            {

                if (strstr(tptr->program, lptr->program) != NULL || strcmp(tptr->program, lptr->program) == 0) // CHECK PROGRAM
                {
                    if (covid_protocol[(time_pref - 6) > 10]) // COVID PROTOCOL
                    {
                        printf("\nBookings at this time exceeded the covid protocol");
                        flag2 = 0;
                    }
                    else if (tptr->slots_booked[(time_pref - 6)] == 0 && flag2 == 1) // SLOT EMPTY CHECK
                    {
                        slot_push(slot_head, lptr, tptr);

                        tptr->slots_booked[time_pref - 6] = 1;
                        time(&rawtime);
                        timeinfo = localtime(&rawtime);

                        strcpy(time_stamp, asctime(timeinfo));

                        printf("\nYour slot has been successsfully booked at %s", time_stamp);
                        printf("_______________________________________________________________________________________________\n");
                        printf("\nMember_Id:%d\nTrainer_Id:%d\nName :%s\nAge:%d\nRemuneration:%d\nProgram Teaching :%s ", lptr->member_ID, tptr->trainer_ID, tptr->name, tptr->age, tptr->renumeration, tptr->program);

                        printf("\nOperating time slots of trainer are: ");
                        for (int i = 0; i < 15; i++)
                        {
                            if (tptr->slots_booked[i] == 1)
                                printf("[%d]", i + 6);
                        }
                        printf("\n_______________________________________________________________________________________________\n");

                        covid_protocol[(time_pref - 6)]++;
                        lptr->slot_booking_index = 1;
                        status = 1;
                        flag2 = 0;
                    }
                    tptr = tptr->next;
                }
            }
            if (status == 0)
            {
                printf("Slot isn't booked ,check your choice and try again ");
                flag2 = 0;
            }
        }
    }
}

void print_hour_wise_list(struct slot_booked **slot_head, struct Node **head_ref, struct trainer_node **head_trainer)
{
    struct trainer_node *tptr = *head_trainer;
    struct slot_booked *sptr = *slot_head;
    for (int i = 0; i < 15; i++)
    {
        while (tptr != NULL)
        {
            if (tptr->slots_booked[i] == 1)
            {
                while (sptr != NULL && sptr->trainer_id == tptr->trainer_ID)
                {
                    sptr = sptr->next;
                }
                printf("_______________________________________________________________________________________________\n");
                printf("\nTrainer Id:%d\n Member_id:%d \nRenumeration:%d\nProgram Enrolled :%s\n Slot Timing:%d \n", tptr->trainer_ID, sptr->member_id, tptr->renumeration, tptr->program, sptr->time);
                printf("_______________________________________________________________________________________________\n");
            }
            tptr = tptr->next;
        }
    }
}

void delete_member(struct Node **head_ref)
{
    int member_id;
    printf("Enter the member id to be deleted");
    scanf("%d", &member_id);

    struct Node *temp = *head_ref, *prev;

    if (temp != NULL && temp->member_ID == member_id)
    {
        *head_ref = temp->next; // Changed head
        free(temp);             // free old head
        return;
    }
    while (temp != NULL && temp->member_ID != member_id) // traversing  till we get the key
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("Invalid member id , please try again");
        return;
    }
    prev->next = temp->next;
    free(temp);
}

void delete_trainer(struct trainer_node **head_ref)
{
    int trainer_id;
    printf("Enter the trainer id to be deleted");
    scanf("%d", &trainer_id);

    struct trainer_node *temp = *head_ref, *prev;

    if (temp != NULL && temp->trainer_ID == trainer_id)
    {
        *head_ref = temp->next; // Changed head
        free(temp);             // free old head
        return;
    }
    while (temp != NULL && temp->trainer_ID != trainer_id) // traversing  till we get the key
    {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL)
    {
        printf("Invalid trainer id , please try again");
        return;
    }
    prev->next = temp->next;
    free(temp); // deallocating memory
}

void search_member(struct Node **head_ref, int member_id)
{
    int flag = 0;
    struct Node *current = *head_ref;
    while (current != NULL && flag == 0)
    {
        if (current->member_ID == member_id)
            flag = 1;
        else
            current = current->next;
    }

    if (current == NULL)
    {
        printf("Member with given id doesn't exist");
    }
    else
    {
        printf("\nMember_Id:%d\nName :%s\nAge:%d\nGender:%s\nProgram Enrolled :%s ", current->member_ID, current->name, current->age, current->gender, current->program);
    }
}

void search_trainer(struct trainer_node **head_ref, int trainer_id)
{
    int flag = 0;
    struct trainer_node *current_trainer = *head_ref;

    while (current_trainer != NULL && flag == 0)
    {
        if (current_trainer->trainer_ID == trainer_id)
            flag = 1;
        else
            current_trainer = current_trainer->next;
    }
    if (current_trainer == NULL)
    {
        printf("Trainer with given id doesn't exist");
    }
    else
    {
        printf("\nTrainer_Id:%d\nName :%s\nAge:%d\nRemuneration:%d\nProgram Teaching :%s ", current_trainer->trainer_ID, current_trainer->name, current_trainer->age, current_trainer->renumeration, current_trainer->program);
    }
}

void print_member_list(struct Node **head_ref)
{
    struct Node *current = *head_ref;
    if (current == NULL)
    {
        printf("The member list is empty. Please add a member first.");
    }
    else
    {
        while (current != NULL)
        {
            printf("_______________________________________________________________________________________________\n");
            printf("\nMember_Id:%d\nName :%s\nAge:%d\nGender:%s\nProgram Enrolled :%s \n", current->member_ID, current->name, current->age, current->gender, current->program);
            printf("_______________________________________________________________________________________________\n");
            current = current->next;
        }
    }
}

void print_trainer_list(struct trainer_node **head_ref)
{
    struct trainer_node *current_trainer = *head_ref;

    if (current_trainer == NULL)
        printf("The trainer list is empty. Please add a trainer first.");
    else
    {
        while (current_trainer != NULL)
        {
            printf("_______________________________________________________________________________________________\n");
            printf("\nTrainer_Id:%d\nName :%s\nAge:%d\nRemuneration:%d\nProgram Teaching :%s ", current_trainer->trainer_ID, current_trainer->name, current_trainer->age, current_trainer->renumeration, current_trainer->program);
            printf("\n_______________________________________________________________________________________________\n");
            current_trainer = current_trainer->next;
        }
    }
}

void swap(trainer *head_ref, trainer *nptr)
{
    struct trainer_node *temp;
    struct trainer_node *current = head_ref;
    struct trainer_node *prev = nptr;

    temp->next = current->next->next;
    prev->next = current->next;
    current->next = temp->next;
}

void print_sorted_renumeration_list(struct trainer_node **head_ref)
{
    struct trainer_node *current_trainer = *head_ref;
    struct trainer_node *nptr = *head_ref;
    int flag = 0, count = 0;
    int i = 0;

    if (current_trainer == NULL)
    {
        printf("\nNo trainer input . Please add a trainer first.");
    }
    else
    {
        do
        {
            while (current_trainer != NULL)
            {
                count++;
                if (current_trainer->renumeration <= current_trainer->next->renumeration)
                {
                    struct trainer_node *nptr = *head_ref;
                    swap(current_trainer, nptr);
                    flag = 1;
                }
                current_trainer = current_trainer->next;
                if (count > 1)
                    nptr = nptr->next;
            }
        } while (flag == 1);
    }
    current_trainer = *head_ref;
    while (current_trainer != NULL)
    {
        printf("_______________________________________________________________________________________________\n");
        printf("\nTrainer_Id:%d\nName :%s\nAge:%d\nRemuneration:%d\nProgram Teaching :%s ", current_trainer->trainer_ID, current_trainer->name, current_trainer->age, current_trainer->renumeration, current_trainer->program);
        printf("_______________________________________________________________________________________________\n");
        current_trainer = current_trainer->next;
    }
}
trainer *copylist(trainer *head)
{
    if (head == NULL)
    {
        return NULL;
    }
    else
    {
        struct trainer_node *newNode = (struct trainer_node *)malloc(sizeof(struct trainer_node));

        strcpy(newNode->name, head->name);
        newNode->age = head->age;
        newNode->renumeration = head->renumeration;
        newNode->trainer_ID = head->trainer_ID;
        strcpy(newNode->program, head->program);

        newNode->next = copylist(head->next); // Cloning linked list recursively
        return newNode;
    }
}

void print_intersection_list(struct Node **head_ref)
{
    struct Node *lptr = *head_ref;
    printf("List of trainers assigned with preffered trainers:");

    while (lptr != NULL)
    {
        if (lptr->pref_trainer_assign_index == 1)
        {
            printf("_______________________________________________________________________________________________\n");
            printf("\nTrainer Id:%d\nMember_Id:%d\nName :%s\nAge:%d\nGender:%s\nProgram Enrolled :%s\nTiming:%d \n", lptr->trainer_pref_id, lptr->member_ID, lptr->name, lptr->age, lptr->gender, lptr->program, lptr->time_preffered);
            printf("_______________________________________________________________________________________________\n");
            lptr = lptr->next;
        }
        else
        {
            lptr = lptr->next;
        }
    }
}

int main()
{
    int choice, flag = 1;
    struct Node *Member_head = NULL;
    struct trainer_node *Trainer_head = NULL;
    struct slot_booked *slot_head = NULL;

    while (flag == 1)
    {
        printf("\t\t Welcome to this program .Strictly give input with CAPSLOCK ON and precise.");
        printf("\n\nSelect your choice:\n1.Add Member\n2.Add trainer\n3.Schedule slot\n5.Delete Member\n6.Delete Trainer\n7.Search member\n8.Search trainer\n9.Print member list\n10.Print trainer list\n11.Print Sorted renumeration list\n12. Print Intersection list ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
        {
            Add_member(&Member_head);
            break;
        }
        case 2:
        {
            Add_trainer(&Trainer_head);
            break;
        }
        case 3:
        {
            schedule_slot(&slot_head, &Member_head, &Trainer_head);
            break;
        }
        case 4:
        {
            print_hour_wise_list(&slot_head, &Member_head, &Trainer_head);
            break;
        }
        case 5:
        {
            delete_member(&Member_head);
            break;
        }
        case 6:
        {
            delete_trainer(&Trainer_head);
            break;
        }
        case 7:
        {
            int member_id;
            printf("Enter the member id :");
            scanf("%d", &member_id);
            search_member(&Member_head, member_id);
            break;
        }
        case 8:
        {
            int trainer_id;
            printf("Enter the trainer id :");
            scanf("%d", &trainer_id);
            search_trainer(&Trainer_head, trainer_id);
            break;
        }
        case 9:
        {
            print_member_list(&Member_head);
            break;
        }
        case 10:
        {
            print_trainer_list(&Trainer_head);
            break;
        }
        case 11:
        {
            struct trainer_node *copy = copylist(Trainer_head);
            print_sorted_renumeration_list(&copy);
            break;
        }
        case 12:
        {
            print_intersection_list(&Member_head);
            break;
        }
        default:
            printf("Invalid input choice");
            break;
        }
        printf("\n Enter 1 to continue , 0 to quit\n");
        scanf("%d", &flag);
    }
    return 0;
}
