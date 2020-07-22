pipeline {
    agent any
    options {
        ansiColor('xterm')
    }
    stages {
        stage('Build') {
			agent {
				docker { image 'conanio/gcc10' }
			}
            steps {
                cache(caches: [[$class: 'ArbitraryFileCache', excludes: '', includes: 'package**', path: '.conan']], maxCacheSize: 0) {
				    sh 'conan create . --build missing --profile ./conan/profiles/default'
                }
                sh 'echo Hello World > testArtifact.rtf'
				archiveArtifacts artifacts: 'testArtifact.rtf', fingerprint: true
            }
        }
    }
}