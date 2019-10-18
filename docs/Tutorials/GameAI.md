# Game AI


## Basic Components

<table>
    <tr>
        <th>AISystem</th>
        <th>AIComponent</th>
        <th>AITask</th>
    </tr>
    <tr>
        <td>
        <ul>
            <li>Knowledge of the game world</li>
            <li>Knowledge of the entities</li>
            <li>Dedicated to a specific area of AI</li>
        </ul>
        </td>
        <td>
        <ul>
            <li>Manages AI states</li>
            <li>Provides interface for adding / removing states</li>
            <li>Manages the current and next AITask</li>
            <li>Provides interface for adding, accessing and removing the current and next AITask</li>
            <li>There can only be one active task at any time</li>
        </ul>
        </td>
        <td>
        <ul>
            <li>Holds the task data</li>
            <li>Identified thorugh its type</id>
        </ul>
        </td>
    </tr>
</table> 