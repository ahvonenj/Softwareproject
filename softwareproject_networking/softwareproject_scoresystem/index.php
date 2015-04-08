<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<title>¯\_(ツ)_/¯</title>
		<link rel="stylesheet" type="text/css" href="style.css">
	</head>
	<?php
		header("Refresh: 10;url='/~G7902/softwareproject_networking/softwareproject_scoresystem/'");
	?>
<body>
	<div id = "wrapper">
		<div id = "content">
			<h1 class = "header"><center>Softwareproject official highscores</center></h1>
			
			<div id "tablecontainer">
				<table class = "scoretable">
					<tr>
						<td class = "tdheader">No.</td>
						<td class = "tdheader">Name:</td>
						<td class = "tdheader">Score:</td>
					</tr>
					
					<?php
						$handle = fopen("scores.txt", "r");
						$scoret = array();
						
						if ($handle) 
						{
							while(($buffer = fgets($handle, 4096)) !== false)
							{
								$scoret[substr($buffer, 0, strpos($buffer, "|"))] = substr($buffer, strpos($buffer, "|")+1);
							}
						}
						arsort($scoret, SORT_NUMERIC);
						
						fclose($handle);
						
						$row = 1;
						
						foreach($scoret as $key => $value)
						{
							if($row <= 25)
							{
								if($key == "" || $value == "")
								{
									continue;
								}
								else
								{
									if($row == 1)
									{
										echo "<tr><td class = 'firstscore'>$row.</td><td class = 'firstscore'>";
										echo $key;
										echo "</td><td class = 'firstscore'>";
										echo $value . " pts";
										echo "</td></tr>";
									}
									else if($row == 2)
									{
										echo "<tr><td class = 'secondscore'>$row.</td><td class = 'secondscore'>";
										echo $key;
										echo "</td><td class = 'secondscore'>";
										echo $value . " pts";
										echo "</td></tr>";
									}
									else if($row == 3)
									{
										echo "<tr><td class = 'thirdscore'>$row.</td><td class = 'thirdscore'>";
										echo $key;
										echo "</td><td class = 'thirdscore'>";
										echo $value . " pts";
										echo "</td></tr>";
									}
									else
									{
										if($row % 2 == 0)
										{
											echo "<tr><td class = 'light'>$row.</td><td class = 'light'>";
											echo $key;
											echo "</td><td class = 'light'>";
											echo $value . " pts";
											echo "</td></tr>";
										}
										else
										{
											echo "<tr><td class = 'dark'>$row.</td><td class = 'dark'>";
											echo $key;
											echo "</td><td class = 'dark'>";
											echo $value . " pts";
											echo "</td></tr>";
										}
									}
									$row++;
								}
							}
							else
							{
								break;
							}
						}
					?>
					</table>
				</div>
				
		</div>
		<img src = "miesk.png" class = "miesko" style = "<?php echo '-webkit-transform:rotate(' . rand(0, 360) . 'deg); transform:rotate(' . rand(0, 360) . 'deg);
		top:' . rand(25, 300) . 'px; left:' . rand(25, 750) . 'px;'?>"/>
	</div>
<body>
<html>

