# Catch The Phrase
## Contributors
- [Angeline B. Basbas](https://github.com/StrayMarimo)
- [Mark Joseph C. Espiritu](https://github.com/Re3ses)
- [Jerome G. Flores](https://github.com/jeromefloresss)
- [Jamie Marie A. Jarme](https://github.com/jamienewtron)
- [Ian Peter L. Lastimosa](https://github.com/LastimosaPeter)


<details open>
    <summary> <h2>Overview</h2> </summary>
    Catch The Phrase is an engaging multiplayer word-guessing game implemented in C using the Raylib library. This documentation serves as a comprehensive guide to understanding the game's architecture, code structure, coding standards, and gameplay mechanics. This documentation is crafted with the intention of providing students, developers, and enthusiasts with a profound insight into the inner workings of "Catch The Phrase," fostering exploration, contribution, and potential enhancements to the project.  </br>  </br>
Whether you're delving into the realms of coding, seeking to understand game development intricacies, or simply exploring the dynamics of multiplayer gaming, this documentation is designed to be your compass in navigating the rich terrain of "Catch The Phrase." Join us as we unravel the layers of this exciting project and discover the fusion of technology, strategy, and creativity encapsulated within.
</details>
<details open>
    <summary> <h2>Usage</h2> </summary>
<h3>Compile: </h3>
<code>./compile.sh </code>
<h3>Run</h3>
<code> cd src </code> <br>
<code>./server <port> </code> <br> 
<code> cd src </code> <br>
<code>./client <host> <port> </code><br>
</details>


<details open>
    <summary> <h2>Main Components </h2> </summary>
    <ol>
        <li><b>Server Component (server.c) </b>
            <ul>
                <li>Responsible for managing the game logic on the server side. </li>
                <li>Handles the setup of socket connections, topic management, and player interactions. </li>
                <li>Prompts Player 1 to select a topic and input a secret phrase. </li>
                <li>Sends topic and phrase information to Player 2. </li>
                <li>Manages the turn-based gameplay and validates player guesses. </li>
                <li>Triggers power-ups based on specific conditions. </li>
                <li>Determines the winner and displays final scores. </li>
            </ul>
        </li>
        <li> <b>Client Component (client.c)</b>
            <ul>
                <li>Manages the game logic on the client side.</li>
                <li>Sets up the client-side socket connection to communicate with the server.</li>
                <li>Receives topic and phrase information from the server.</li>
                <li>Displays blank phrases for the chosen topic.</li>
                <li>Allows Player 2 to guess letters or marked spots during their turn.</li>
                <li>Triggers power-ups based on specific conditions.</li>
                <li>Displays updated information, including revealed letters and power-up effects.</li>
                <li>Determines the winner and displays final scores.</li>
            </ul> 
        </li>
        <li> <b>Common Utilities (common_utils.c)</b>
            <ul>
                <li>Contains utility functions for validation, error handling, encryption, and string manipulation.</li>
                <li>Provides functions for encryption of phrases, capitalization, and handling system messages.</li>
            </ul> 
        </li>
        <li> <b>Socket Connection (socket_connection.c)</b>
            <ul>
                <li>Handles socket setup, connection, and management on the server side.</li>
                <li>Creates and binds sockets, handles new client connections, and establishes connections to clients.</li>
            </ul> 
        </li>
        <li> <b>Sound Manager (sound_manager.c)</b>
            <ul>
                <li>Deals with loading and unloading audio files for the game.</li>
                <li>Manages the audio aspects of the game, including loading sound effects for various events.</li>
            </ul> 
        </li>
    </ol>
</details>

<details open>
    <summary> <h2>Features</h2> </summary>
    <ol>
        <li><b>Client-Server Architecture</b> </br>
           Catch The Phrase employs a client-server architecture, where the server manages the game state and logic, and the client interacts with the user and displays the game's visual elements. This separation allows for scalability, as multiple clients can connect to the same server.
        </li>
        <li> <b>Turn-Based Gameplay</b> </br>
          The turn-based design pattern ensures that players take alternating turns, allowing for a fair and organized gameplay experience. This pattern enables players to guess phrases or letters during their respective turns.
        </li>
        <li> <b> State Management</b> </br>
           The game maintains different states, including TITLE, GAMEPLAY, and GAMEOVER. State management ensures that the game progresses smoothly from the title screen to gameplay and handles the endgame conditions.
        </li>
        <li> Message Passing</b> </br>
            Communication between the server and client is achieved through message passing using sockets. Messages include topics, phrases, acknowledgments, and other game-related information.
        </li>
        <li> <b>Power-Up System</b> </br>
            The game incorporates a power-up system triggered by specific conditions, such as correctly guessing a marked spot or achieving consecutive correct guesses. This adds an element of surprise and strategy to the gameplay.
        </li>
          <li> <b> User Interface</b> </br>
           The graphical user interface (GUI) is managed by the Raylib library, providing a visually appealing and intuitive interface for players. The GUI updates dynamically based on user actions and game events.
        </li>
    </ol>
</details>

<details open>
    <summary> <h2>Directory Structure</h2> </summary>
    <ul>
        <li>src/: This directory contains the source code files for the Catch The Phrase game. </li>
        <li>common_utils.c: Manages utility functions for common tasks, error handling, encryption, and string manipulation. </li>
        <li>socket_communication.c: Handles message sending and receiving over sockets between the client and server. </li>
        <li>socket_connection.c: Manages the setup and management of socket connections on the server side. </li>
        <li>sound_manager.c: Deals with loading and unloading audio files for the game, handling the audio aspects of the game. </li>
        <li>client.c: Implements the client-side game logic, user interface, and interaction. </li>
        <li>server.c: Implements the server-side game logic, topic management, and player interactions. </li>
        <li>.gitignore: Specifies patterns to exclude from version control, ensuring that unnecessary files and directories are not tracked. </li>
        <li>README.md: Contains instructions on how to compile and run the game, project description, and usage information. </li>
        <li>compile.sh: A Bash script automating the compilation process, installing dependencies, cloning the Raylib library, and compiling the server and client programs. </li>
    <ul>

</details>




