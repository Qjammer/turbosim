pipeline {
    agent any

    stages {
		stage('')
        stage('Build') {
			agent {
				docker { image 'conanio/gcc10' }
			}
            steps {
                sh 'echo Hello World > testArtifact.rtf'
				sh 'conan install -if build . --build missing -s build_type=Release'
				archiveArtifacts artifacts: 'testArtifact.rtf', fingerprint: true
            }
        }
    }
}