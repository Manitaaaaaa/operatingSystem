#include <stdio.h>

int main()
{
    // Basic memory configuration
    int pageSize;
    int totalFrames;
    int totalPages;

    printf("Enter Page Size (KB): ");
    scanf("%d", &pageSize);

    printf("Enter Total Frames: ");
    scanf("%d", &totalFrames);

    printf("Enter Total Pages: ");
    scanf("%d", &totalPages);

    // Page reference string
    int pages[totalPages];

    printf("Enter Page Reference String:\n");

    for(int i = 0; i < totalPages; i++)
    {
            scanf("%d", &pages[i]);
    }

    // Memory frames
    int frames[totalFrames];

    // FIFO pointer
    int nextFrame = 0;

    // Counters
    int pageFaults = 0;
    int pageHits = 0;

    // Initialize frames as empty
    for(int i = 0; i < totalFrames; i++)
    {
        frames[i] = -1;
    }

    printf("Page Size : %d KB\n", pageSize);
    printf("Total Frames : %d\n\n", totalFrames);

    printf("Page Reference String : ");

    for(int i = 0; i < totalPages; i++)
    {
        printf("%d ", pages[i]);
    }

    printf("\n\nFIFO Page Replacement\n\n");

    for(int i = 0; i < totalPages; i++)
    {
        int found = 0;

        // Check if page is already in memory
        for(int j = 0; j < totalFrames; j++)
        {
            if(frames[j] == pages[i])
            {
                found = 1;
                pageHits++;

                printf("Page %d -> Hit\n", pages[i]);
                break;
            }
        }

        // If page is not found
        if(found == 0)
        {
            if(nextFrame < totalFrames)
            {
                frames[nextFrame] = pages[i];

                printf("Loaded Page %d into Frame %d\n",
                       pages[i],
                       nextFrame + 1);

                nextFrame++;
            }
            else
            {
                int replace = pageFaults % totalFrames;

                printf("Replacing Page %d with Page %d\n",
                       frames[replace],
                       pages[i]);

                frames[replace] = pages[i];
            }

            pageFaults++;
            // Display current memory frames
            printf("Frames : ");

            for(int j = 0; j < totalFrames; j++)
            {
               if(frames[j] == -1)
                          printf("- ");
               else
                          printf("%d ", frames[j]);
            }

             printf("\n\n");
        }
    }

        printf("\nCurrent Memory Frames\n");

        for(int i = 0; i < totalFrames; i++)
    {
        printf("Frame %d : %d\n", i + 1, frames[i]);
    }

             printf("FIF0 Page Hits : %d\n", pageHits);
             printf("FIFO Page Faults : %d\n", pageFaults);

    float hitRatio = (float)pageHits / totalPages;
    float missRatio = (float)pageFaults / totalPages;

    printf("FIFO Hit Ratio : %.2f\n", hitRatio);
    printf("FIFO Miss Ratio : %.2f\n", missRatio);

    // LRU Page Replacement

    printf("\n\nLRU Page Replacement\n");

    // LRU frames
    int lruFrames[totalFrames];

    // Store last used position
    int lastUsed[totalFrames];

    // Initialize LRU frames
    for(int i = 0; i < totalFrames; i++)
    {
          lruFrames[i] = -1;
          lastUsed[i] = -1;
     }

     // LRU counters
     int lruHits = 0;
     int lruFaults = 0;

     printf("\nInitial LRU Frames:\n");

     for(int i = 0; i < totalFrames; i++)
     {
           printf("Frame %d : %d\n", i + 1, lruFrames[i]);
     }
     printf("\n\nLoading Pages using LRU\n\n");

     for(int i = 0; i < totalPages; i++)
     {
     int found = 0;

     // Check if page already exists
     for(int j = 0; j < totalFrames; j++)
     {
        if(lruFrames[j] == pages[i])
        {
            found = 1;
            lruHits++;
            lastUsed[j] = i;

            printf("Page %d -> Hit\n", pages[i]);
            break;
        }
    }

    // Page not found
    if(found == 0)
{
    int index = -1;

    // Find an empty frame
    for(int j = 0; j < totalFrames; j++)
    {
        if(lruFrames[j] == -1)
        {
            index = j;
            break;
        }
    }

    // If no empty frame, replace least recently used page
    if(index == -1)
    {
        index = 0;

        for(int j = 1; j < totalFrames; j++)
        {
            if(lastUsed[j] < lastUsed[index])
            {
                index = j;
            }
        }

        printf("Replacing Page %d with Page %d\n",
               lruFrames[index],
               pages[i]);
    }
    else
    {
        printf("Loaded Page %d into Frame %d\n",
               pages[i],
               index + 1);
    }

    lruFrames[index] = pages[i];
    lastUsed[index] = i;
    lruFaults++;
}

    // Display frames
    printf("Frames : ");

    for(int j = 0; j < totalFrames; j++)
    {
        if(lruFrames[j] == -1)
            printf("- ");
        else
            printf("%d ", lruFrames[j]);
    }

    printf("\n\n");
 }
    printf("LRU Page Hits : %d\n", lruHits);
    printf("LRU Page Faults : %d\n", lruFaults);

    float lruHitRatio = (float)lruHits / totalPages;
    float lruMissRatio = (float)lruFaults / totalPages;

    printf("LRU Hit Ratio : %.2f\n", lruHitRatio);
    printf("LRU Miss Ratio : %.2f\n", lruMissRatio);

    return 0;
}
