import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:guidedcariot/firebase_options.dart';

Future<void> main() async {
  WidgetsFlutterBinding.ensureInitialized();

  await Firebase.initializeApp(
    options: DefaultFirebaseOptions.currentPlatform,
  );
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Guided Car Iot',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: const MyHomePage(title: 'Dashboard'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key, required this.title});

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  int _speed = 0;
  String _direction = "stopped"; // "forward", "backward", "left", "right"
  bool _isManual = true;

  final DatabaseReference _database = FirebaseDatabase.instance.ref('vehicle');

  void _saveData() async {
    try {
      await _database.set({
        'speed': _speed,
        'direction': _direction,
        'mode': _isManual ? "manual" : "automatic",
      });
      print('Data updated successfully');
    } catch (e) {
      print('Failed to update data: $e');
    }
  }

  void _increaseSpeed() {
    setState(() {
      _speed++;
      _saveData();
    });
  }
  
  void _decreaseSpeed() {
    setState(() {
      if (_speed > 0) _speed--;
      _saveData();
    });
  }

  void _setDirection(String direction) {
    setState(() {
      _direction = direction;
      if (_direction == "stopped") _speed = 0;
      _saveData();
    });
  }

  void _toggleMode() {
    setState(() {
      _isManual = !_isManual;
      _saveData();
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        title: Text(widget.title),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text(
              'Mode: ${_isManual ? "Manual" : "Automatic"}',
              style: Theme.of(context).textTheme.headlineMedium,
            ),
            ElevatedButton(
              onPressed: _toggleMode,
              child: Text(_isManual ? 'Switch to Automatic' : 'Switch to Manual'),
            ),
            const SizedBox(height: 32),
            // Control Buttons
            ElevatedButton(
              onPressed: _isManual ? () => _setDirection("stopped") : null,
              child: const Icon(Icons.stop),
              style: ElevatedButton.styleFrom(
                backgroundColor: Colors.red, // Background color
                foregroundColor: Colors.white, // Text/Icon color
                shape: const CircleBorder(),
                padding: const EdgeInsets.all(24),
              ),
            ),
            const SizedBox(height: 32),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                ElevatedButton(
                  onPressed: _isManual ? () => _setDirection("left") : null,
                  child: const Icon(Icons.arrow_back),
                ),
                const SizedBox(width: 16),
                ElevatedButton(
                  onPressed: _isManual ? () => _setDirection("right") : null,
                  child: const Icon(Icons.arrow_forward),
                ),
              ],
            ),
            const SizedBox(height: 16),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                ElevatedButton(
                  onPressed: _isManual ? () => _setDirection("forward") : null,
                  child: const Icon(Icons.arrow_upward),
                ),
                const SizedBox(width: 16),
                ElevatedButton(
                  onPressed: _isManual ? () => _setDirection("backward") : null,
                  child: const Icon(Icons.arrow_downward),
                ),
              ],
            ),
            const SizedBox(height: 32),
            // Speed Control
            Text(
              'Speed: $_speed',
              style: Theme.of(context).textTheme.headlineMedium,
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                ElevatedButton(
                  onPressed: _decreaseSpeed,
                  child: const Icon(Icons.remove),
                ),
                const SizedBox(width: 16),
                ElevatedButton(
                  onPressed: _increaseSpeed,
                  child: const Icon(Icons.add),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
}
