<?php	
	require('../currentversion.php');

	if($_POST['name'] || $_POST['score'] || $_POST['auth'])
	{
		$file = fopen("ipad.txt", "a");
		fwrite($file, "[X]			" . date("G:i:s") . "			" . $_SERVER['REMOTE_ADDR'] . "\r\n");
		fclose($file);
	}
	else
	{
		$file = fopen("ipad.txt", "a");
		fwrite($file, "[ ]			" . date("G:i:s") . "			" . $_SERVER['REMOTE_ADDR'] . "\r\n");
		fclose($file);
	}
	
	if($_SERVER['REMOTE_ADDR'] != "72.47.255.39") //Teemu ip-banned
	{
		if($_POST['name'] && $_POST['score'] && $_POST['auth'] && $_POST['antihack'])
		{		
			$name = $_POST["name"];
			$score = $_POST["score"];
			$auth = $_POST["auth"];
			
			
			$handle = fopen("scores.txt", "r");
			$scoret = array();
			
			$i = 0;
			if ($handle) 
			{
				while(($buffer = fgets($handle, 4096)) !== false)
				{
					$scoret[$i] = substr($buffer, 0, strpos($buffer, "|")) . "|" . substr($buffer, strpos($buffer, "|")+1);	
					$i++;
				}
			}
			fclose($handle);
			
			$doWrite = 0;
			$wasAlready = 0;
			foreach($scoret as $key => $value)
			{			
				if(strtolower($name) == strtolower(substr($value, 0, strpos($value, "|")))) //Toimii
				{
					if($score > (int)substr($value, strpos($value, "|")+1))
					{
						$scoret[$key] = $name . "|" . $score . "\n";
						$doWrite = 1;
						$wasAlready = 1;
					}
					else
					{
						//header("Debug3: name = " . substr($value, 0, strpos($value, "|")) . "score: " . $score . " > " . substr($value, strpos($value, "|")+1));
						$doWrite = 0;
						$wasAlready = 0;
					}
					break;
				}
				else
				{
					$wasAlready = 0;
					$doWrite = 1;
					continue;
				}
				$doWrite = 1;
			}
			
			if($doWrite == 1)
			{
				$clientantihack == $_POST['antihack'];
				$serverantihack == md5( pow(score, 243316681) );
				
				if($auth == $currentversion && $clientantihack == $serverantihack)
				{
					$numItems = count($scoret);
					$j = 0;
				
					if($wasAlready == 1)
					{
						$file = fopen("scores.txt", "w");
						
						foreach($scoret as $key => $value)
						{
							fwrite($file, substr($value, 0, strpos($value, "|")) . "|" . substr($value, strpos($value, "|")+1)) . "\r\n";
						}
					}
					else
					{
						$file = fopen("scores.txt", "a");
						fwrite($file, $name . "|" . $score . "\n");

					}
					fclose($file);
					
					header("Success: 1");
					return true;
				}
				else
				{
					header("Success: 0");
					return false;
				}
			}
		}
		else
		{
			echo "<font color = 'red'>Access denied via browser interface.</br>This incident has been logged.</font>";
			return false;
		}
	}
	else
	{
		echo "Saved! :)";
	}
?>