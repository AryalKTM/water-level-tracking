import 'package:firebase_database/firebase_database.dart';

// class FirebaseService {
//   // final DatabaseReference _databaseReference = FirebaseDatabase.instance.ref();
// }

Stream<DatabaseEvent> getDepth() {
  DatabaseReference depth = FirebaseDatabase.instance.ref('/test/depth');
  return depth.onChildAdded;
}

Stream<DatabaseEvent> getStatus() {
  DatabaseReference status = FirebaseDatabase.instance.ref("/test/relay_status");
  return status.onValue;
}
